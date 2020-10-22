/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <vector>

enum class ToolManagerState
{
  idle,
  scan,
  tlp_config
};

class ToolManager
{
private:
  ToolManagerState tool_mngr_state_;

public:
  ToolManager();
  ~ToolManager();
  bool setState(ToolManagerState state);
};