/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <ros/ros.h>
#include <ros/console.h>

#include <agribot_msgs/Tags.h>
#include <agribot_msgs/ToolManagerState.h>
#include <agribot_msgs/ToolManagerControlSource.h>
#include <agribot_msgs/RfidPower.h>

#include <vector>
#include <memory>

#include "agribot_tools_management/Scanner.h"
#include "agribot_tools_management/TplController.h"
#include "agribot_tools_management/TplArbiter.h"

namespace tool_manager
{
class ToolManagerNode
{
 private:
  std::shared_ptr<ros::NodeHandle> node_handle_;
  ros::ServiceServer tool_state_srv_;
  ros::ServiceServer tpl_control_arbiter_srv_;
  ros::ServiceClient tags_srv_client_;
  ros::ServiceClient rfid_power_srv_client_;

  Scanner scan_handler_;
  TplArbiter tpl_control_arbiter_;
  TplController tpl_control_handler_;

  ReturnCode getTagsSrvCall(std::vector<uint16_t>& detected_tags);  // NOLINT(runtime/references)
  ReturnCode setPwrSrvCall(double pwr);
  bool performScanAndPwrAdjustments(const agribot_msgs::ToolManagerState::Request& req,  // NOLINT(runtime/references)
                                    agribot_msgs::ToolManagerState::Response& res);      // NOLINT(runtime/references)
  bool setTpl();
  bool checkErrCode(ReturnCode err, agribot_msgs::ToolManagerState::Response& res);   // NOLINT(runtime/references)
  bool stateRequestService(agribot_msgs::ToolManagerState::Request& req,              // NOLINT(runtime/references)
                           agribot_msgs::ToolManagerState::Response& res);            // NOLINT(runtime/references)
  bool controlArbiterService(agribot_msgs::ToolManagerControlSource::Request& req,    // NOLINT(runtime/references)
                             agribot_msgs::ToolManagerControlSource::Response& res);  // NOLINT(runtime/references)
 public:
  ToolManagerNode(int argc, char** argv);
  ~ToolManagerNode();
  void run();
};
}  // namespace tool_manager
