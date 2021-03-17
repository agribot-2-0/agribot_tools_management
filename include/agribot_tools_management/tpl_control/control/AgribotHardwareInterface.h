/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include "agribot_tools_management/tpl_control/control/AgribotHardware.h"

#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/robot_hw.h>
#include <joint_limits_interface/joint_limits_interface.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <joint_limits_interface/joint_limits_rosparam.h>
#include <controller_manager/controller_manager.h>
#include <boost/scoped_ptr.hpp>
#include <ros/ros.h>

namespace tool_manager
{
class AgribotHardwareInterface : public AgribotHardware
{
public:
  AgribotHardwareInterface(ros::NodeHandle& nh);
  ~AgribotHardwareInterface();
  void init();
  void update(const ros::TimerEvent& e);
  void read();
  void write(ros::Duration elapsed_time);

protected:
  // tr1cpp::TR1 tr1; agribot valve values publish handler
  ros::NodeHandle nh_;
  ros::Timer non_realtime_loop_;
  ros::Duration control_period_;
  ros::Duration elapsed_time_;
  hardware_interface::PositionJointInterface positionJointInterface;
  joint_limits_interface::PositionJointSoftLimitsInterface positionJointSoftLimitsInterface;
  double loop_hz_;
  boost::shared_ptr<controller_manager::ControllerManager> controller_manager_;
  double p_error_, v_error_, e_error_;
  std::string _logInfo;
};
}  // namespace tool_manager
