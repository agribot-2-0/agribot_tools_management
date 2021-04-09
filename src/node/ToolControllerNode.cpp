/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#include "agribot_tools_management/node/ToolControllerNode.h"

#include "agribot_tools_management/common/JointsNames.h"
#include "agribot_tools_management/common/MsgStructTransformFunctions.h"
#include "agribot_tools_management/common/TplControlTopicName.h"

namespace tool_manager
{
void ToolControllerNode::initDataMaps(const std::vector<std::string>& names)
{
  for (const auto& name : names)
  {
    feedback_values_[name] = 0.0;
    control_values_[name] = 0.0;
  }
}

void ToolControllerNode::frontTplFeedbackCallback(const agribot_msgs::AgribotTool::ConstPtr& msg)
{
  feedback_values_[joint_names.front_tpl_upper_hitch_prismatic] = msg->tpl_height;
  feedback_values_[joint_names.front_tpl_lower_hitches_revolute] = msg->tpl_angle;
}

void ToolControllerNode::backTplFeedbackCallback(const agribot_msgs::AgribotTool::ConstPtr& msg)
{
  feedback_values_[joint_names.back_tpl_upper_hitch_prismatic] = msg->tpl_height;
  feedback_values_[joint_names.back_tpl_lower_hitches_revolute] = msg->tpl_angle;
}

void ToolControllerNode::frontTplSetpointCallback(const agribot_msgs::AgribotTool::ConstPtr& msg)
{
  front_tpl_setpoint_ = msg_struct_transform_functions::toolMsgToTplConfig(msg); // todo: zmienic na referencje
}

void ToolControllerNode::backTplSetpointCallback(const agribot_msgs::AgribotTool::ConstPtr& msg)
{
  back_tpl_setpoint_ = msg_struct_transform_functions::toolMsgToTplConfig(msg); // todo: zmienic na referencje
}

void ToolControllerNode::publishControlMessages() const
{
  agribot_msgs::AgribotTool msg;
  msg.tpl_angle = control_values_.at(joint_names.front_tpl_lower_hitches_revolute);
  msg.tpl_height = control_values_.at(joint_names.front_tpl_upper_hitch_prismatic);
  front_tpl_control_pub_.publish(msg);
  msg.tpl_height = control_values_.at(joint_names.back_tpl_upper_hitch_prismatic);
  msg.tpl_angle = control_values_.at(joint_names.back_tpl_lower_hitches_revolute);
  back_tpl_control_pub_.publish(msg);
}

void ToolControllerNode::publishCommands() const
{
  std_msgs::Float64 msg;
  msg.data = front_tpl_setpoint_.tpl_angle;
  front_tpl_lower_hitches_revolute_command_pub_.publish(msg);
  msg.data = front_tpl_setpoint_.tpl_height;
  front_tpl_upper_hitch_prismatic_command_pub_.publish(msg);
  msg.data = back_tpl_setpoint_.tpl_angle;
  back_tpl_lower_hitches_revolute_command_pub_.publish(msg);
  msg.data = back_tpl_setpoint_.tpl_height;
  back_tpl_upper_hitch_prismatic_command_pub_.publish(msg);
}

void ToolControllerNode::waitForRobotDescriptionParameter() const
{
  ros::Rate rate(1);
  while (!node_handle_->hasParam("/robot_description"))
  {
    ros::spinOnce();
    rate.sleep();
  }
}

ToolControllerNode::ToolControllerNode(int argc, char** argv)
{
  ros::init(argc, argv, "agribot_tools_control_node");
  node_handle_ = std::make_shared<ros::NodeHandle>();
  // /robot_descritpion parameter is needed by controller manager
  waitForRobotDescriptionParameter();
  std::vector<std::string> joints_names;
  try
  {
    joints_names = ros_common_functions::getRosParam<std::vector<std::string>>("/agribot/hardware_interface/joints");
  }
  catch (const agribot_exceptions::ParameterNotSet& ex)
  {
    ROS_ERROR_STREAM("[ToolControllerNode] " << ex.what());
    ros::shutdown();
  }
  initDataMaps(joints_names);
  agribot_hardware_interface_ = std::make_unique<AgribotHardwareInterface>(std::move(joints_names));
  control_manager_ =
      std::make_unique<controller_manager::ControllerManager>(&(*agribot_hardware_interface_), *node_handle_);

  front_tpl_feedback_sub_ = node_handle_->subscribe(tpl_control_topic_name.front_feedback, 1,
                                                    &ToolControllerNode::frontTplFeedbackCallback, this);
  back_tpl_feedback_sub_ = node_handle_->subscribe(tpl_control_topic_name.back_feedback, 1,
                                                   &ToolControllerNode::backTplFeedbackCallback, this);
  front_tpl_setpoint_sub_ = node_handle_->subscribe(tpl_control_topic_name.front_setpoint, 1,
                                                    &ToolControllerNode::frontTplSetpointCallback, this);
  back_tpl_setpoint_sub_ = node_handle_->subscribe(tpl_control_topic_name.back_setpoint, 1,
                                                   &ToolControllerNode::backTplSetpointCallback, this);

  front_tpl_control_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>(tpl_control_topic_name.front_control, 1);
  back_tpl_control_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>(tpl_control_topic_name.back_control, 1);
  front_tpl_lower_hitches_revolute_command_pub_ =
      node_handle_->advertise<std_msgs::Float64>(tpl_control_topic_name.front_tpl_lower_hitches_revolute_control, 1);
  front_tpl_upper_hitch_prismatic_command_pub_ =
      node_handle_->advertise<std_msgs::Float64>(tpl_control_topic_name.front_tpl_upper_hitch_prismatic_control, 1);
  back_tpl_lower_hitches_revolute_command_pub_ = node_handle_->advertise<std_msgs::Float64>(
      tpl_control_topic_name.back_tpl_lower_hitches_revolute_control_command, 1);
  back_tpl_upper_hitch_prismatic_command_pub_ =
      node_handle_->advertise<std_msgs::Float64>(tpl_control_topic_name.back_tpl_upper_hitch_prismatic_control, 1);
}

void ToolControllerNode::run()
{
  ros::AsyncSpinner spinner(1);
  spinner.start();
  ros::Rate rate(10);
  ros::Time time = ros::Time::now();
  ros::Time previous_time = ros::Time::now();
  while (ros::ok())
  {
    // ros control
    time = ros::Time::now();
    const ros::Duration period = time - previous_time;
    agribot_hardware_interface_->read(feedback_values_);
    control_manager_->update(ros::Time::now(), period);
    agribot_hardware_interface_->write(control_values_);
    previous_time = time;
    publishControlMessages();  // publish control valves values (output from ros_control)
    publishCommands();         // publish setpoints, from inner class structers to controller command topics (input to
                               // ros_control)
    ros::spinOnce();
    rate.sleep();
  }
}
}  // namespace tool_manager
