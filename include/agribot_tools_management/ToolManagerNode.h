/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <ros/ros.h>
#include <ros/console.h>

#include <agribot_msgs/Tags.h>
#include <agribot_msgs/ToolState.h>
#include <agribot_msgs/RfidPower.h>
#include "agribot_tools_management/ToolManager.h"

class ToolManagerNode
{
private:
  ros::NodeHandle* node_handle_; /*TODO: use some kind of smart pointer instead*/
  ros::ServiceServer tool_state_service_;
  ros::ServiceServer tags_service_;
  ros::ServiceServer rfid_power_service_;

  ToolManager tool_manager_;
  bool state_request_service(agribot_msgs::ToolState::Request& req,    // NOLINT(runtime/references)
                             agribot_msgs::ToolState::Response& res);  // NOLINT(runtime/references)
public:
  ToolManagerNode();
  ToolManagerNode(ros::NodeHandle* nh);
  ~ToolManagerNode();
  void run();
};
