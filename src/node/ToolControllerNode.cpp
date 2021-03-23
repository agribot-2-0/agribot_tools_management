/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include "agribot_tools_management/node/ToolControllerNode.h"

namespace tool_manager
{
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
}

void ToolControllerNode::run()
{
  ros::AsyncSpinner spinner(1);
  spinner.start();
  ros::Rate rate(1);
  ros::Time time;
  ros::Time prev_time;
  while (ros::ok())
  {
    // Basic bookkeeping to get the system time in order to compute the control period.
    time = ros::Time::now();
    const ros::Duration period = time - prev_time;
    agribot_hardware_interface_->read();
    control_manager_->update(ros::Time::now(), period);
    agribot_hardware_interface_->write();
    prev_time = time;
    ros::spinOnce();
    rate.sleep();
  }
}
}  // namespace tool_manager