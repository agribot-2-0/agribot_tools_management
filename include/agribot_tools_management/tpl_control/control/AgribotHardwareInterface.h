/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>

#include <joint_limits_interface/joint_limits_interface.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <joint_limits_interface/joint_limits_rosparam.h>

#include <agribot_common/TplConfig.h>

namespace tool_manager
{
class AgribotHardwareInterface : public hardware_interface::RobotHW
{
 private:
  std::vector<std::string> joint_names_;
  std::unordered_map<std::string, double> effort_command_;
  std::unordered_map<std::string, double> position_command_;
  std::unordered_map<std::string, double> position_;
  std::unordered_map<std::string, double> velocity_;
  std::unordered_map<std::string, double> effort_;

  hardware_interface::JointStateInterface joint_state_interface_;
  hardware_interface::PositionJointInterface joint_position_interface_;
  hardware_interface::EffortJointInterface joint_effort_interface_;

 public:
  AgribotHardwareInterface(std::vector<std::string>&& joint_names);
  ~AgribotHardwareInterface();
  void read(const std::unordered_map<std::string, double>& feedbacks);
  void write(std::unordered_map<std::string, double>& commands) const;
};
}  // namespace tool_manager
