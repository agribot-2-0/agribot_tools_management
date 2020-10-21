/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <vector>

class ToolManager
{
private:
  uint8_t state_;

public:
  const struct
  {
    uint8_t idle;
    uint8_t scan;
    uint8_t tlp_config;
  } tool_mngr_state_ = { 0, 1, 2 };

  ToolManager();
  ~ToolManager();
  bool setState(uint8_t state);
};