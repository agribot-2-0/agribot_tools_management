/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#include "agribot_tools_management/node/ToolControllerNode.h"

int main(int argc, char** argv)
{
  tool_manager::ToolControllerNode tool_control_node(argc, argv);
  tool_control_node.run();
  return 0;
}
