/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#pragma once

#include <ros/ros.h>
#include <ros/console.h>
// Custom Callback Queue
#include <ros/advertise_options.h>
#include <ros/callback_queue.h>

#include <agribot_msgs/Tag.h>
#include <agribot_msgs/Tags.h>
#include <agribot_msgs/ToolManagerScan.h>
#include <agribot_msgs/ToolManagerControlSource.h>
#include <agribot_msgs/AgribotTool.h>
#include <agribot_msgs/RfidPower.h>

#include <string>
#include <vector>
#include <memory>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "agribot_tools_management/scanner/Scanner.h"
#include "agribot_tools_management/tpl_control/control_arbiter/TplControlBroadcaster.h"
#include "agribot_tools_management/tpl_control/control_arbiter/TplArbiter.h"
#include "agribot_tools_management/common/ReturnCode.h"
#include "agribot_tools_management/common/ControlSource.h"

namespace tool_manager
{
class ToolManagerNode
{
 private:
  int ros_loop_rate_ = 10;
  std::shared_ptr<ros::NodeHandle> node_handle_;
  ros::ServiceServer tool_state_srv_;
  ros::ServiceServer tpl_control_arbiter_srv_;
  ros::ServiceClient tags_srv_client_;
  ros::ServiceClient rfid_power_srv_client_;

  ros::Subscriber tpl_control_manual_topic_front_sub_;
  ros::Subscriber tpl_control_manual_topic_back_sub_;
  ros::Subscriber tpl_control_navi_topic_front_sub_;
  ros::Subscriber tpl_control_navi_topic_back_sub_;
  ros::Publisher tpl_control_destination_front_pub_;
  ros::Publisher tpl_control_destination_back_pub_;

  Scanner scan_handler_;
  TplArbiter tpl_control_arbiter_;
  TplControlBroadcaster tpl_control_broadcast_handler_;

  // Custom Callback Queue
  ros::CallbackQueue queue_;
  boost::thread callback_queue_thread_;
  void QueueThread();

  std::string retCodeToString(ReturnCode ret);
  ReturnCode getTagsSrvCall(std::vector<agribot_msgs::Tag>& detected_tags);  // NOLINT(runtime/references)
  ReturnCode setPwrSrvCall(double pwr);
  bool performScanAndPwrAdjustments(const agribot_msgs::ToolManagerScan::Request& req,  // NOLINT(runtime/references)
                                    agribot_msgs::ToolManagerScan::Response& res);      // NOLINT(runtime/references)
  bool checkErrCode(ReturnCode err, agribot_msgs::ToolManagerScan::Response& res);      // NOLINT(runtime/references)
  bool stateRequestService(agribot_msgs::ToolManagerScan::Request& req,                 // NOLINT(runtime/references)
                           agribot_msgs::ToolManagerScan::Response& res);               // NOLINT(runtime/references)
  bool controlArbiterService(agribot_msgs::ToolManagerControlSource::Request& req,      // NOLINT(runtime/references)
                             agribot_msgs::ToolManagerControlSource::Response& res);    // NOLINT(runtime/references)

  void republishControlMsg() const;

  void cmdToolsFrontManualCallback(const agribot_msgs::AgribotTool::ConstPtr& tool_msg);
  void cmdToolsBackManualCallback(const agribot_msgs::AgribotTool::ConstPtr& tool_msg);
  void cmdToolsFrontNaviCallback(const agribot_msgs::AgribotTool::ConstPtr& tool_msg);
  void cmdToolsBackNaviCallback(const agribot_msgs::AgribotTool::ConstPtr& tool_msg);

 public:
  ToolManagerNode(int argc, char** argv);
  ~ToolManagerNode();
  void run();
};
}  // namespace tool_manager
