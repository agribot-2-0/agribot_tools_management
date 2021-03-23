/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <ros/ros.h>
#include <ros/console.h>

#include <memory>
#include <vector>
#include <string_view>

#include <agribot_common/TplConfig.h>
#include <agribot_common/CommonFunctions/ros_common_functions.h>
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
  // Structures for storing feedback and command values
  TplConfig front_tpl_feedback_;
  TplConfig back_tpl_feedback_;
  TplConfig front_tpl_setpoint_;
  TplConfig back_tpl_setpoint_;
  TplConfig front_tpl_control_;
  TplConfig back_tpl_control_;

 public:
  ToolControllerNode(int argc, char** argv);
  void run();
};
}  // namespace tool_manager
