/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

namespace tool_manager
{
ToolManagerNode::ToolManagerNode(int argc, char** argv)
{
  ros::init(argc, argv, "agribot_tools_management_node");
  node_handle_ = std::make_shared<ros::NodeHandle>();
  tool_state_srv_ =
      node_handle_->advertiseService("agribot/tool_management_state", &ToolManagerNode::stateRequestService, this);
  tags_srv_client_ = node_handle_->serviceClient<agribot_msgs::Tags>("/agribot/rfid/tags");
  rfid_power_srv_client_ = node_handle_->serviceClient<agribot_msgs::RfidPower>("/agribot/rfid/antennas/power");
}

bool ToolManagerNode::checkErrCode(ErrorCode err,
                                   agribot_msgs::ToolManagerState::Response& res)  // NOLINT(runtime/references)
{
  if (err == ErrorCode::Success)
  {
    res.result = res.SUCCESS;
    return true;
  }
  else
  {
    res.result = res.FAIL;
    ROS_ERROR_STREAM("[TOOL MANAGEMENT] Request failed with error code: " << static_cast<int>(err));
    return false;
  }
}

ErrorCode ToolManagerNode::getTagsSrvCall(std::vector<uint16_t>* detected_tags)
{
  agribot_msgs::Tags tags_msg;
  tags_msg.request.selected_antenna = static_cast<uint8_t>(scan_handler_.getAnt());
  detected_tags->clear();
  if (tags_srv_client_.call(tags_msg))
  {
    for (int i = 0; i < tags_msg.response.ids.size(); i++)
    {
      detected_tags->push_back(tags_msg.response.ids[i]);
    }
    return ErrorCode::Success;
  }
  else
  {
    return ErrorCode::TagsServiceError;
  }
}

ErrorCode ToolManagerNode::setPwrSrvCall(double pwr)
{
  agribot_msgs::RfidPower pwr_msg;
  pwr_msg.request.power_value = pwr;
  pwr_msg.request.selected_antenna = scan_handler_.getAnt();
  if (rfid_power_srv_client_.call(pwr_msg))
  {
    return ErrorCode::Success;
  }
  else
  {
    return ErrorCode::PowerServiceError;
  }
}

bool ToolManagerNode::stateRequestService(agribot_msgs::ToolManagerState::Request& req,   // NOLINT(runtime/references)
                                          agribot_msgs::ToolManagerState::Response& res)  // NOLINT(runtime/references)
{
  ErrorCode ret = ErrorCode::Success;
  ROS_INFO_STREAM("enter callback");
  switch (req.requested_state)
  {
    case req.SCAN_FRONT:
    case req.SCAN_BACK:
      scan_handler_.setAnt(req.requested_state);
      ROS_INFO_STREAM("Scan triggered from from ant " << static_cast<int>(scan_handler_.getAnt()));
      while (!scan_handler_.entirePowerRangeInspected())
      {
        if (!checkErrCode(getTagsSrvCall(&(res.ids)), res))  // if there is sth wrong with tags service
        {
          return false;
        }
        ROS_INFO_STREAM("Tags detected: " << static_cast<int>(res.ids.size()));
        scan_handler_.setTagsCnt(res.ids.size());
        if (scan_handler_.scanCorrect())
        {
          break;
        }
        else
        {
          double pwr = scan_handler_.adjustPower();
          if (!checkErrCode(setPwrSrvCall(pwr), res))  // if there is sth wrong with pwr service
          {
            return false;
          }
        }
      }
      break;
    case req.SET_TPL:
      // TBD
      break;
  }
  return true;
}

void ToolManagerNode::run()
{
  ros::AsyncSpinner spinner(2);
  spinner.start();
  ros::waitForShutdown();
}

ToolManagerNode::~ToolManagerNode()
{
}

}  // namespace tool_manager
