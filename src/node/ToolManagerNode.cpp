/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#include "agribot_tools_management/node/ToolManagerNode.h"

#include "agribot_tools_management/common/MsgStructTransformFunctions.h"

namespace tool_manager
{
ToolManagerNode::ToolManagerNode(int argc, char** argv)
{
  ros::init(argc, argv, "agribot_tools_management_node");
  node_handle_ = std::make_shared<ros::NodeHandle>();

  // Services
  tool_state_srv_ =
      node_handle_->advertiseService("agribot/tool_management/scan", &ToolManagerNode::stateRequestService, this);
  tpl_control_arbiter_srv_ = node_handle_->advertiseService("agribot/tool_management/control_source",
                                                            &ToolManagerNode::controlArbiterService, this);
  tags_srv_client_ = node_handle_->serviceClient<agribot_msgs::Tags>("/agribot/rfid/tags");
  rfid_power_srv_client_ = node_handle_->serviceClient<agribot_msgs::RfidPower>("/agribot/rfid/antennas/power");

  // TPL config publishers
  auto [topic_front, topic_back] = tpl_control_arbiter_.getFrontBackDestinationTopics();
  tpl_control_destination_front_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>(topic_front, 1);
  tpl_control_destination_back_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>(topic_back, 1);

  // TPL config subs manual
  std::tie(topic_front, topic_back) = tpl_control_arbiter_.getFrontBackSourceTopics(ControlSource::Manual);
  ros::SubscribeOptions sub_cmd_tool_front_man = ros::SubscribeOptions::create<agribot_msgs::AgribotTool>(
      topic_front, 1, boost::bind(&ToolManagerNode::cmdToolsFrontManualCallback, this, _1), ros::VoidPtr(), &queue_);
  ros::SubscribeOptions sub_cmd_tool_back_man = ros::SubscribeOptions::create<agribot_msgs::AgribotTool>(
      topic_back, 1, boost::bind(&ToolManagerNode::cmdToolsBackManualCallback, this, _1), ros::VoidPtr(), &queue_);
  tpl_control_manual_topic_front_sub_ = node_handle_->subscribe(sub_cmd_tool_front_man);
  tpl_control_manual_topic_back_sub_ = node_handle_->subscribe(sub_cmd_tool_back_man);

  // TPL config subs navi
  std::tie(topic_front, topic_back) = tpl_control_arbiter_.getFrontBackSourceTopics(ControlSource::Navigation);
  ros::SubscribeOptions sub_cmd_tool_front_nav = ros::SubscribeOptions::create<agribot_msgs::AgribotTool>(
      topic_front, 1, boost::bind(&ToolManagerNode::cmdToolsFrontNaviCallback, this, _1), ros::VoidPtr(), &queue_);
  ros::SubscribeOptions sub_cmd_tool_back_nav = ros::SubscribeOptions::create<agribot_msgs::AgribotTool>(
      topic_back, 1, boost::bind(&ToolManagerNode::cmdToolsBackNaviCallback, this, _1), ros::VoidPtr(), &queue_);
  tpl_control_navi_topic_front_sub_ = node_handle_->subscribe(sub_cmd_tool_front_nav);
  tpl_control_navi_topic_back_sub_ = node_handle_->subscribe(sub_cmd_tool_back_nav);

  // Set timeout for the arbiter
  int timeout;
  node_handle_->param<int>("/tpl_arbiter_timeout", timeout, 100);  // [ms]
  tpl_control_arbiter_.setTimeout(timeout, ros_loop_rate_);

  // Custom callback queue
  this->callback_queue_thread_ = boost::thread(boost::bind(&ToolManagerNode::QueueThread, this));
}

void ToolManagerNode::QueueThread()
{
  static const double timeout = 0.01;
  while (node_handle_->ok())
  {
    queue_.callAvailable(ros::WallDuration(timeout));
  }
}

std::string ToolManagerNode::retCodeToString(ReturnCode ret)
{
  switch (ret)
  {
    case ReturnCode::Success:
      return "Success";
    case ReturnCode::TagsServiceError:
      return "TagsServiceError";
    case ReturnCode::PowerServiceError:
      return "PowerServiceError";
    default:
      return "Unknown error";
  }
}

bool ToolManagerNode::checkErrCode(ReturnCode err,
                                   agribot_msgs::ToolManagerScan::Response& res)  // NOLINT(runtime/references)
{
  if (err != ReturnCode::Success)
  {
    res.result = res.FAIL;
    ROS_ERROR_STREAM("[ToolManagement] Request failed with error: " << retCodeToString(err));
    return false;
  }
  res.result = res.SUCCESS;
  return true;
}

ReturnCode ToolManagerNode::getTagsSrvCall(std::vector<agribot_msgs::Tag>& detected_tags)
{
  agribot_msgs::Tags tags_msg;
  tags_msg.request.selected_antenna = static_cast<uint8_t>(scan_handler_.getAnt());
  detected_tags.clear();
  if (!tags_srv_client_.call(tags_msg))
  {
    return ReturnCode::TagsServiceError;
  }
  detected_tags = std::vector<agribot_msgs::Tag>(tags_msg.response.ids);
  return ReturnCode::Success;
}

ReturnCode ToolManagerNode::setPwrSrvCall(double pwr)
{
  agribot_msgs::RfidPower pwr_msg;
  pwr_msg.request.power_value = pwr;
  pwr_msg.request.selected_antenna = static_cast<uint8_t>(scan_handler_.getAnt());
  if (!rfid_power_srv_client_.call(pwr_msg))
  {
    return ReturnCode::PowerServiceError;
  }
  return ReturnCode::Success;
}

bool ToolManagerNode::performScanAndPwrAdjustments(
    const agribot_msgs::ToolManagerScan::Request& req,  // NOLINT(runtime/references)
    agribot_msgs::ToolManagerScan::Response& res)       // NOLINT(runtime/references)
{
  scan_handler_.setAnt(static_cast<Antenna>(req.requested_state));
  ROS_INFO_STREAM("[ToolManagement] Scan triggered for ant: " << static_cast<int>(scan_handler_.getAnt()));
  while (!scan_handler_.entirePowerRangeInspected())
  {
    if (!checkErrCode(getTagsSrvCall(res.ids), res))  // if there is sth wrong with tags service
    {
      return false;
    }
    ROS_INFO_STREAM("[ToolManagement] Tags detected: " << static_cast<int>(res.ids.size()));
    scan_handler_.setTagsCnt(res.ids.size());
    if (scan_handler_.scanCorrect())
    {
      return true;
    }
    double pwr = scan_handler_.adjustPower();
    if (!checkErrCode(setPwrSrvCall(pwr), res))  // if there is sth wrong with pwr service
    {
      return false;
    }
  }
  return true;
}

bool ToolManagerNode::stateRequestService(agribot_msgs::ToolManagerScan::Request& req,   // NOLINT(runtime/references)
                                          agribot_msgs::ToolManagerScan::Response& res)  // NOLINT(runtime/references)
{
  switch (req.requested_state)
  {
    case req.SCAN_FRONT:
      return performScanAndPwrAdjustments(req, res);
    case req.SCAN_BACK:
      return performScanAndPwrAdjustments(req, res);
    default:
      break;
  }
  return false;
}

bool ToolManagerNode::controlArbiterService(
    agribot_msgs::ToolManagerControlSource::Request& req,   // NOLINT(runtime/references)
    agribot_msgs::ToolManagerControlSource::Response& res)  // NOLINT(runtime/references)
{
  tpl_control_arbiter_.monitorResetTimeoutCnts();
  switch (req.source)
  {
    case req.MANUAL:
      tpl_control_arbiter_.setTplControlSource(ControlSource::Manual);
      res.result = res.SUCCESS;
      ROS_WARN_STREAM("[ToolManagement] Control source changed to Manual");
      return true;
    case req.NAVIGATION:
      tpl_control_arbiter_.setTplControlSource(ControlSource::Navigation);
      res.result = res.SUCCESS;
      ROS_WARN_STREAM("[ToolManagement] Control source changed to Navigation");
      return true;
    case req.NONE:
      tpl_control_arbiter_.setTplControlSource(ControlSource::None);
      res.result = res.SUCCESS;
      ROS_WARN_STREAM("[ToolManagement] Control source changed to None");
      return true;
    default:
      res.result = res.FAIL;
      return false;
  }
}

void ToolManagerNode::republishControlMsg() const
{
  agribot_msgs::AgribotTool msg;
  auto [cfg_front, cfg_back] =
      tpl_control_broadcast_handler_.getConfigsFrontBack(tpl_control_arbiter_.getTplControlSource());
  msg = msg_struct_transform_functions::tplConfigToToolMsg(cfg_front);
  tpl_control_destination_front_pub_.publish(msg);
  msg = msg_struct_transform_functions::tplConfigToToolMsg(cfg_back);
  tpl_control_destination_back_pub_.publish(msg);
}

void ToolManagerNode::cmdToolsFrontManualCallback(const agribot_msgs::AgribotTool::ConstPtr& tool_msg)
{
  tpl_control_broadcast_handler_.setConfig(ConfigIndex::FrontManual,
                                           msg_struct_transform_functions::toolMsgToTplConfig(tool_msg));
  tpl_control_arbiter_.monitorResetTimeoutCnt(ConfigIndex::FrontManual);
}

void ToolManagerNode::cmdToolsBackManualCallback(const agribot_msgs::AgribotTool::ConstPtr& tool_msg)
{
  tpl_control_broadcast_handler_.setConfig(ConfigIndex::BackManual,
                                           msg_struct_transform_functions::toolMsgToTplConfig(tool_msg));
  tpl_control_arbiter_.monitorResetTimeoutCnt(ConfigIndex::BackManual);
}

void ToolManagerNode::cmdToolsFrontNaviCallback(const agribot_msgs::AgribotTool::ConstPtr& tool_msg)
{
  tpl_control_broadcast_handler_.setConfig(ConfigIndex::FrontNavigation,
                                           msg_struct_transform_functions::toolMsgToTplConfig(tool_msg));
  tpl_control_arbiter_.monitorResetTimeoutCnt(ConfigIndex::FrontNavigation);
}

void ToolManagerNode::cmdToolsBackNaviCallback(const agribot_msgs::AgribotTool::ConstPtr& tool_msg)
{
  tpl_control_broadcast_handler_.setConfig(ConfigIndex::BackNavigation,
                                           msg_struct_transform_functions::toolMsgToTplConfig(tool_msg));
  tpl_control_arbiter_.monitorResetTimeoutCnt(ConfigIndex::BackNavigation);
}

void ToolManagerNode::run()
{
  // AsyncSpinner is used to be able to call services from within other services (in scanner functionality)
  ros::AsyncSpinner spinner(2);
  spinner.start();
  ros::Rate rate(ros_loop_rate_);
  while (ros::ok())
  {
    if (tpl_control_arbiter_.sourceSelected())
    {
      if (tpl_control_arbiter_.timeoutsExceeded())
      {
        tpl_control_arbiter_.setTplControlSource(ControlSource::None);
        ROS_WARN_STREAM("[ToolManagement] Control source changed to None");
        ROS_ERROR_STREAM("[ToolManagement] TPL control topic inactivity timeout");
      }
      republishControlMsg();
    }
    ros::spinOnce();
    rate.sleep();
  }
}

ToolManagerNode::~ToolManagerNode()
{
}

}  // namespace tool_manager
