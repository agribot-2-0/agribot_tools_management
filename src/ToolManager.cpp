/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManager.h"
#include <cstdint>

ToolManager::ToolManager()
{
  state_ = tool_mngr_state_.idle;
}

bool ToolManager::setState(uint8_t state)
{
  state_ = state;
}

ToolManager::~ToolManager()
{
}