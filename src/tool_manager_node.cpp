/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

int main(int argc, char** argv)
{
  ToolManagerNode tool_mngr_node(argc, argv);
  tool_mngr_node.run();
}