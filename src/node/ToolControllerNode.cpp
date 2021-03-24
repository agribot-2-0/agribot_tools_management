/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include "agribot_tools_management/node/ToolControllerNode.h"
#include "agribot_tools_management/common/TplControlTopicName.h"
#include "agribot_tools_management/common/MsgStructTransformFunctions.h"

namespace tool_manager
{
void ToolControllerNode::frontTplFeedbackCallback(const agribot_msgs::AgribotTool::ConstPtr& msg)
{
  front_tpl_feedback_ = msg_struct_transform_functions::toolMsgToTplConfig(msg);
}

void ToolControllerNode::backTplFeedbackCallback(const agribot_msgs::AgribotTool::ConstPtr& msg)
{
  back_tpl_feedback_ = msg_struct_transform_functions::toolMsgToTplConfig(msg);
}

void ToolControllerNode::frontTplSetpointCallback(const agribot_msgs::AgribotTool::ConstPtr& msg)
{
  front_tpl_setpoint_ = msg_struct_transform_functions::toolMsgToTplConfig(msg);
}

void ToolControllerNode::backTplSetpointCallback(const agribot_msgs::AgribotTool::ConstPtr& msg)
{
  back_tpl_setpoint_ = msg_struct_transform_functions::toolMsgToTplConfig(msg);
}

void ToolControllerNode::publishControlMessages() const
{
  agribot_msgs::AgribotTool msg;
  msg_struct_transform_functions::tplConfigToToolMsg(front_tpl_control_, msg);
  front_tpl_control_pub_.publish(msg);
  msg_struct_transform_functions::tplConfigToToolMsg(back_tpl_control_, msg);
  back_tpl_control_pub_.publish(msg);
}

void ToolControllerNode::publishCommands() const
{
  // [IMPORTANT] In order for controller to work proerly, the setpoint for prismatic joint needs to be set
  // as an negative values! (this is caused by joints description in /robot_description param)
  std_msgs::Float64 msg;
  msg.data = front_tpl_setpoint_.tpl_angle;
  front_tpl_lower_hitches_revolute_command_pub_.publish(msg);
  msg.data = -front_tpl_setpoint_.tpl_height;
  front_tpl_upper_hitch_prismatic_command_pub_.publish(msg);
  msg.data = back_tpl_setpoint_.tpl_angle;
  back_tpl_lower_hitches_revolute_command_pub_.publish(msg);
  msg.data = -back_tpl_setpoint_.tpl_height;
  back_tpl_upper_hitch_prismatic_command_pub_.publish(msg);
}

ToolControllerNode::ToolControllerNode(int argc, char** argv)
{
  ros::init(argc, argv, "agribot_tools_control_node");
  node_handle_ = std::make_shared<ros::NodeHandle>();
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
  ros::Rate rate(10);  // todo(mwiadoms): read loop from rosparam
  ros::Time time;
  ros::Time previous_time;
  while (ros::ok())
  {
    // ros control
    time = ros::Time::now();
    const ros::Duration period = time - previous_time;
    agribot_hardware_interface_->read(front_tpl_feedback_, back_tpl_feedback_);
    control_manager_->update(ros::Time::now(), period);
    agribot_hardware_interface_->write(front_tpl_control_, back_tpl_control_);
    previous_time = time;
    // publish control values
    publishControlMessages();  // publish control valves values (output from ros_control)
    publishCommands();         // publish setpoints, from inner class structers to controller command topics (input to
                               // ros_control)
    ros::spinOnce();
    rate.sleep();
  }
}
}  // namespace tool_manager