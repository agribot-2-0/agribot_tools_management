
#include <sstream>
#include "agribot_tools_management/tpl_control/control/AgribotHardwareInterface.h"
#include <joint_limits_interface/joint_limits_interface.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <joint_limits_interface/joint_limits_rosparam.h>

using namespace hardware_interface;
using joint_limits_interface::JointLimits;
using joint_limits_interface::PositionJointSoftLimitsHandle;
using joint_limits_interface::PositionJointSoftLimitsInterface;
using joint_limits_interface::SoftJointLimits;

namespace tool_manager
{
AgribotHardwareInterface::AgribotHardwareInterface(ros::NodeHandle& nh) : nh_(nh)
{
  init();
  controller_manager_.reset(new controller_manager::ControllerManager(this, nh_));

  nh_.param("/agribot/hardware_interface/loop_hz", loop_hz_, 0.1);
  ROS_DEBUG_STREAM_NAMED("constructor", "Using loop freqency of " << loop_hz_ << " hz");
  ros::Duration update_freq = ros::Duration(1.0 / loop_hz_);
  non_realtime_loop_ = nh_.createTimer(update_freq, &AgribotHardwareInterface::update, this);

  ROS_INFO_NAMED("hardware_interface", "Loaded generic_hardware_interface.");
}

AgribotHardwareInterface::~AgribotHardwareInterface()
{
}

void AgribotHardwareInterface::init()
{
  nh_.getParam("/agribot/hardware_interface/joints", joint_names_);
  if (joint_names_.size() == 0)
  {
    ROS_FATAL_STREAM_NAMED("init", "No joints found on parameter server for controller. Did you load the proper yaml "
                                   "file?");
  }
  num_joints_ = joint_names_.size();

  // Resize vectors
  joint_position_.resize(num_joints_);
  joint_velocity_.resize(num_joints_);
  joint_effort_.resize(num_joints_);
  joint_position_command_.resize(num_joints_);
  joint_velocity_command_.resize(num_joints_);
  joint_effort_command_.resize(num_joints_);

  // Initialize controller
  for (int i = 0; i < num_joints_; ++i)
  {
    //tr1cpp::Joint joint = tr1.getJoint(joint_names_[i]);

    ROS_DEBUG_STREAM_NAMED("constructor", "Loading joint name: " << joint_names_[i]);

    //nh_.getParam("/tr1/joint_offsets/" + joint.name, joint.angleOffset);
    //nh_.getParam("/tr1/joint_read_ratio/" + joint.name, joint.readRatio);
    //tr1.setJoint(joint);

    // Create joint state interface
    JointStateHandle jointStateHandle(joint_names_[i], &joint_position_[i], &joint_velocity_[i], &joint_effort_[i]);
    joint_state_interface_.registerHandle(jointStateHandle);

    // Create position joint interface
    JointHandle jointPositionHandle(jointStateHandle, &joint_position_command_[i]);
    JointLimits limits;
    SoftJointLimits softLimits;
    if (getJointLimits(joint_names_[i], nh_, limits) == false)
    {
      ROS_ERROR_STREAM("Cannot set joint limits for " << joint_names_[i]);
    }
    else
    {
      PositionJointSoftLimitsHandle jointLimitsHandle(jointPositionHandle, limits, softLimits);
      positionJointSoftLimitsInterface.registerHandle(jointLimitsHandle);
    }
    position_joint_interface_.registerHandle(jointPositionHandle);

    // Create velocity joint interface
    // JointHandle jointVelocityHandle(jointStateHandle, &joint_velocity_command_[i]);
    // effort_joint_interface_.registerHandle(jointVelocityHandle);

    // Create effort joint interface
    JointHandle jointEffortHandle(jointStateHandle, &joint_effort_command_[i]);
    effort_joint_interface_.registerHandle(jointEffortHandle);
  }

  registerInterface(&joint_state_interface_);
  registerInterface(&position_joint_interface_);
  //registerInterface(&velocity_joint_interface_);
  registerInterface(&effort_joint_interface_);
  registerInterface(&positionJointSoftLimitsInterface);
}

void AgribotHardwareInterface::update(const ros::TimerEvent& e)
{
  _logInfo = "\n";
  _logInfo += "Joint Position Command:\n";
  for (int i = 0; i < num_joints_; i++)
  {
    std::ostringstream jointPositionStr;
    jointPositionStr << joint_position_command_[i];
    _logInfo += "  " + joint_names_[i] + ": " + jointPositionStr.str() + "\n";
  }

  elapsed_time_ = ros::Duration(e.current_real - e.last_real);

  read();
  controller_manager_->update(ros::Time::now(), elapsed_time_);
  write(elapsed_time_);

  ROS_INFO_STREAM(_logInfo);
}

// TODO: get current data from topic, which contains TPL encoder data
// and nextly
void AgribotHardwareInterface::read()
{
  _logInfo += "Joint State:\n";
  for (int i = 0; i < num_joints_; i++)
  {
    // tr1cpp::Joint joint = tr1.getJoint(joint_names_[i]);

    // if (joint.getActuatorType() == ACTUATOR_TYPE_MOTOR)
    // {
    //   joint_position_[i] = joint.readAngle();

    //   std::ostringstream jointPositionStr;
    //   jointPositionStr << joint_position_[i];
    //   _logInfo += "  " + joint.name + ": " + jointPositionStr.str() + "\n";
    // }

    joint_position_[i] = i;

    //tr1.setJoint(joint);
  }
}

void AgribotHardwareInterface::write(ros::Duration elapsed_time)
{
  positionJointSoftLimitsInterface.enforceLimits(elapsed_time);

  _logInfo += "Joint Effort Command:\n";
  for (int i = 0; i < num_joints_; i++)
  {
    //tr1cpp::Joint joint = tr1.getJoint(joint_names_[i]);
    // if (joint_effort_command_[i] > 1) joint_effort_command_[i] = 1;
    // if (joint_effort_command_[i] < -1) joint_effort_command_[i] = -1;

    double effort = joint_effort_command_[i];
    uint8_t duration = 15;

    // if (joint.getActuatorType() == 1)
    // {  // servo
    //   double previousEffort = joint.getPreviousEffort();
    //   effort += previousEffort;
    // }

    //joint.actuate(effort, duration);

    std::ostringstream jointEffortStr;
    jointEffortStr << joint_effort_command_[i];
    _logInfo += "  " + joint_names_[i] + ": " + jointEffortStr.str() + "\n";
  }
}
}  // namespace 
