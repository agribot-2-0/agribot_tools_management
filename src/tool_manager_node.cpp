/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "agribot_tools_management_node");
  ros::NodeHandle nh("~");
  ToolManagerNode tool_mngr_node(&nh);
  tool_mngr_node.run();
}