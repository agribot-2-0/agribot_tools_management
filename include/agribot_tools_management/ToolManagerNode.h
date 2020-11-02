/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <ros/ros.h>
#include <ros/console.h>

#include <agribot_msgs/Tags.h>
#include <agribot_msgs/ToolManagerState.h>
#include <agribot_msgs/RfidPower.h>

#include <vector>
#include <memory>

#include "agribot_tools_management/Scanner.h"
#include "agribot_tools_management/TplController.h"

namespace tool_manager
{
class ToolManagerNode
{
 private:
  Scanner scan_handler_;
  TplController tpl_control_handler_;

  std::shared_ptr<ros::NodeHandle> node_handle_;
  ros::ServiceServer tool_state_srv_;
  ros::ServiceClient tags_srv_client_;
  ros::ServiceClient rfid_power_srv_client_;

  ErrorCode getTagsSrvCall(std::vector<uint16_t>* detected_tags);
  ErrorCode setPwrSrvCall(double pwr);
  bool checkErrCode(ErrorCode err, agribot_msgs::ToolManagerState::Response& res);  // NOLINT(runtime/references)
  bool stateRequestService(agribot_msgs::ToolManagerState::Request& req,            // NOLINT(runtime/references)
                           agribot_msgs::ToolManagerState::Response& res);          // NOLINT(runtime/references)
 public:
  ToolManagerNode(int argc, char** argv);
  ~ToolManagerNode();
  void run();
};
}  // namespace tool_manager
