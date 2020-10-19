/*
* Copyright (C) 2020 Agribot. All rights reserved.
*/

#include "agribot_tools_management/ToolManager.h"
#include <cstdint>

constexpr uint8_t idle = 0;
constexpr uint8_t scan = 1;
constexpr uint8_t tools_manuals = 2;

ToolManager::ToolManager()
{
  state_ = idle;
}

bool ToolManager::setState(uint8_t state)
{
  state_ = state;
}