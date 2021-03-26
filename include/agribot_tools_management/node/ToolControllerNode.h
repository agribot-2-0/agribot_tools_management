/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <ros/ros.h>
#include <ros/console.h>

#include <memory>
#include <vector>
#include <string_view>

#include <agribot_common/TplConfig.h>
#include <agribot_common/CommonFunctions/ros_common_functions.h>

#include <agribot_msgs/AgribotTool.h>
#include <std_msgs/Float64.h>

#include <controller_manager/controller_manager.h>
#include <agribot_tools_management/tpl_control/control/AgribotHardwareInterface.h>

namespace tool_manager
{
class ToolControllerNode
{
private:
  std::shared_ptr<ros::NodeHandle> node_handle_;
  // ros_control:
  std::unique_ptr<AgribotHardwareInterface> agribot_hardware_interface_;
  std::unique_ptr<controller_manager::ControllerManager> control_manager_;
  // pubs/subs:
  ros::Subscriber front_tpl_feedback_sub_;
  ros::Subscriber back_tpl_feedback_sub_;
  ros::Subscriber front_tpl_setpoint_sub_;
  ros::Subscriber back_tpl_setpoint_sub_;
  ros::Publisher front_tpl_control_pub_;
  ros::Publisher back_tpl_control_pub_;
  ros::Publisher back_tpl_lower_hitches_revolute_command_pub_;
  ros::Publisher back_tpl_upper_hitch_prismatic_command_pub_;
  ros::Publisher front_tpl_lower_hitches_revolute_command_pub_;
  ros::Publisher front_tpl_upper_hitch_prismatic_command_pub_;
  // Structures for storing feedback and setpoints
  TplConfig front_tpl_setpoint_;
  TplConfig back_tpl_setpoint_;
  std::unordered_map<std::string, double> feedback_values_;
  std::unordered_map<std::string, double> control_values_;

  void initDataMaps(const std::vector<std::string>& names);
  void frontTplFeedbackCallback(const agribot_msgs::AgribotTool::ConstPtr& msg);
  void backTplFeedbackCallback(const agribot_msgs::AgribotTool::ConstPtr& msg);
  void frontTplSetpointCallback(const agribot_msgs::AgribotTool::ConstPtr& msg);
  void backTplSetpointCallback(const agribot_msgs::AgribotTool::ConstPtr& msg);
  void publishControlMessages() const;
  void publishCommands() const;

public:
  ToolControllerNode(int argc, char** argv);
  void run();
};
}  // namespace tool_manager
