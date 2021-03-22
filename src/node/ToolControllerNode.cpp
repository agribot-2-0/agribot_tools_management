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

  agribot_hardware_interface_ = std::make_unique<AgribotHardwareInterface>(new AgribotHardwareInterface(*node_handle_));
  control_manager_ = std::make_unique<controller_manager::ControllerManager>(
      new controller_manager::ControllerManager(&(*agribot_hardware_interface_), *node_handle_));
}

void ToolControllerNode::run()
{
  // NOTE: We run the ROS loop in a separate thread as external calls such
  // as service callbacks to load controllers can block the (main) control loop
  ros::AsyncSpinner spinner(4);  // Use 4 threads
  spinner.start();
  ros::waitForShutdown();
}
}  // namespace tool_manager