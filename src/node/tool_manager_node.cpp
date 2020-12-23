/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/node/ToolManagerNode.h"

int main(int argc, char** argv)
{
  tool_manager::ToolManagerNode tool_mngr_node(argc, argv);
  tool_mngr_node.run();
  return 0;
}
