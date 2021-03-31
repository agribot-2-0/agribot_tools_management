/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <sstream>
#include "agribot_tools_management/tpl_control/control/AgribotHardwareInterface.h"
#include "agribot_tools_management/common/JointsNames.h"

namespace tool_manager
{
AgribotHardwareInterface::AgribotHardwareInterface(std::vector<std::string>&& joint_names)
  : joint_names_{ std::move(joint_names) }
{
  // Initialize controller
  for (const auto& name : joint_names_)
  {
    ROS_ERROR_STREAM("[constructor] Loading joint name: " << name);
    // joint state handle for storing data read by encoders:
    hardware_interface::JointStateHandle joint_state_handle(name, &position_[name], &velocity_[name], &effort_[name]);
    joint_state_interface_.registerHandle(joint_state_handle);
    // joint handle for position control:
    hardware_interface::JointHandle joint_position_handle(joint_state_handle, &position_command_[name]);
    joint_position_interface_.registerHandle(joint_position_handle);
    // joint handle for effort control:
    hardware_interface::JointHandle joint_effort_handle(joint_state_handle, &effort_command_[name]);
    joint_effort_interface_.registerHandle(joint_effort_handle);
  }

  registerInterface(&joint_state_interface_);
  registerInterface(&joint_position_interface_);
  registerInterface(&joint_effort_interface_);
}

AgribotHardwareInterface::~AgribotHardwareInterface()
{
}

// // TODO: get current data from topic, which contains TPL encoder data
void AgribotHardwareInterface::read(const std::unordered_map<std::string, double>& feedbacks)
{
  position_[joint_names.front_tpl_lower_hitches_revolute] = feedbacks.at(joint_names.front_tpl_lower_hitches_revolute);
  position_[joint_names.front_tpl_upper_hitch_prismatic] = feedbacks.at(joint_names.front_tpl_upper_hitch_prismatic);
  position_[joint_names.back_tpl_lower_hitches_revolute] = feedbacks.at(joint_names.back_tpl_lower_hitches_revolute);
  position_[joint_names.back_tpl_upper_hitch_prismatic] = feedbacks.at(joint_names.back_tpl_upper_hitch_prismatic);
}

void AgribotHardwareInterface::write(std::unordered_map<std::string, double>& commands) const
{
  commands[joint_names.front_tpl_lower_hitches_revolute] =
      effort_command_.at(joint_names.front_tpl_lower_hitches_revolute);
  commands[joint_names.front_tpl_upper_hitch_prismatic] =
      effort_command_.at(joint_names.front_tpl_upper_hitch_prismatic);
  commands[joint_names.back_tpl_lower_hitches_revolute] =
      effort_command_.at(joint_names.back_tpl_lower_hitches_revolute);
  commands[joint_names.back_tpl_upper_hitch_prismatic] = effort_command_.at(joint_names.back_tpl_upper_hitch_prismatic);
}

}  // namespace tool_manager
