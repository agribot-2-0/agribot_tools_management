
#include <sstream>
#include "agribot_tools_management/tpl_control/control/AgribotHardwareInterface.h"

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

// void AgribotHardwareInterface::update(const ros::TimerEvent& e)
// {
//   _logInfo = "\n";
//   _logInfo += "Joint Position Command:\n";
//   for (int i = 0; i < num_joints_; i++)
//   {
//     std::ostringstream jointPositionStr;
//     jointPositionStr << joint_position_command_[i];
//     _logInfo += "  " + joint_names_[i] + ": " + jointPositionStr.str() + "\n";
//   }

//   elapsed_time_ = ros::Duration(e.current_real - e.last_real);

//   read();
//   // controller_manager_->update(ros::Time::now(), elapsed_time_); move to outer ROS loop
//   write(elapsed_time_);

//   ROS_INFO_STREAM(_logInfo);
// }

// // TODO: get current data from topic, which contains TPL encoder data
void AgribotHardwareInterface::read()
{
  for (const auto& name : joint_names_)
  {
    position_[name] = 0.32;
  }
//   _logInfo += "Joint State:\n";
//   for (int i = 0; i < num_joints_; i++)
//   {
//     joint_position_[i] = i;

//     std::ostringstream jointPositionStr;
//     jointPositionStr << joint_position_[i];
//     _logInfo += "  " + joint_names_[i] + ": " + jointPositionStr.str() + "\n";

//     // tr1.setJoint(joint);
//   }
}

void AgribotHardwareInterface::write()
{
//   positionJointSoftLimitsInterface.enforceLimits(elapsed_time);

//   _logInfo += "Joint Effort Command:\n";
//   for (int i = 0; i < num_joints_; i++)
//   {
//     // tr1cpp::Joint joint = tr1.getJoint(joint_names_[i]);
//     // if (joint_effort_command_[i] > 1) joint_effort_command_[i] = 1;
//     // if (joint_effort_command_[i] < -1) joint_effort_command_[i] = -1;

//     double effort = joint_effort_command_[i];

//     // uint8_t duration = 15;
//     // if (joint.getActuatorType() == 1)
//     // {  // servo
//     //   double previousEffort = joint.getPreviousEffort();
//     //   effort += previousEffort;
//     // }

//     // joint.actuate(effort, duration);

//     std::ostringstream jointEffortStr;
//     jointEffortStr << joint_effort_command_[i];
//     _logInfo += "  " + joint_names_[i] + ": " + jointEffortStr.str() + "\n";
//   }

  for (const auto& name : joint_names_)
  {
    ROS_ERROR_STREAM(name << " effort command: " << effort_command_[name]);
    ROS_ERROR_STREAM(name << " positi command: " << position_command_[name]);
  }
}

}  // namespace tool_manager
