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

namespace tool_manager
{
class AgribotHardwareInterface : public hardware_interface::RobotHW
{
 private:
  std::vector<std::string> joint_names_;
  // Data member arrays to store the controller commands which are sent to the
  // robot's resources (joints, actuators)
  std::unordered_map<std::string, double> effort_command_;
  std::unordered_map<std::string, double> position_command_;
  // Data member arrays to store the state of the robot's resources (joints, sensors)
  std::unordered_map<std::string, double> position_;
  std::unordered_map<std::string, double> velocity_;
  std::unordered_map<std::string, double> effort_;

  // hardware_interface::JointStateInterface gives read access to all joint values
  // without conflicting with other controllers.
  hardware_interface::JointStateInterface joint_state_interface_;
  // hardware_interface::PositionJointInterface inherits from
  // hardware_interface::JointCommandInterface and is used for reading and writing
  // joint positions. Because this interface reserves the joints for write access,
  // conflicts with other controllers writing to the same joints might occure.
  // To only read joint positions, avoid conflicts using
  // hardware_interface::JointStateInterface.
  hardware_interface::PositionJointInterface joint_position_interface_;
  hardware_interface::EffortJointInterface joint_effort_interface_;

 public:
  AgribotHardwareInterface(std::vector<std::string>&& joint_names);
  ~AgribotHardwareInterface();
  // get most current data from proper topics
  void read();
  // update command values
  void update();
  // publish control values to proper topics
  void write();
};
}  // namespace tool_manager
