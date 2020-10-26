/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

namespace tool_manager
{
  enum class ErrorCode
  {
    Success = 0
  };
  class ToolManagerTask
  {
  public:
    virtual ErrorCode perform() = 0;
  };
}  // namespace tool_manager