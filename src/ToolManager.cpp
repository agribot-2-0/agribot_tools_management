/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManager.h"
#include <cstdint>

ToolManager::ToolManager()
{
  tool_mngr_state_ = ToolManagerState::idle;
}

bool ToolManager::setState(ToolManagerState state)
{
  tool_mngr_state_ = state;
}

ToolManager::~ToolManager()
{
}