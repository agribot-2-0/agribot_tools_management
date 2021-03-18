/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include "agribot_tools_management/tpl_control/control/AgribotHardwareInterface.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "agribot_hardware_interface");
  ros::NodeHandle nh;

  // NOTE: We run the ROS loop in a separate thread as external calls such
  // as service callbacks to load controllers can block the (main) control loop
  ros::AsyncSpinner spinner(4);  // Use 4 threads
  spinner.start();
  tool_manager::AgribotHardwareInterface tpl_controller(nh);
  ros::waitForShutdown();
  return 0;
}
