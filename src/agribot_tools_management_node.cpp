/*
* Copyright (C) 2020 Agribot. All rights reserved.
*/

#include <ros/ros.h>
#include <ros/console.h>
#include "agribot_tools_management/ToolManager.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "agribot_tools_management_node");
  ros::NodeHandle n;
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
}
