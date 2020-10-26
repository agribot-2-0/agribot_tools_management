/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

namespace tool_manager
{
  ToolManagerNode::TplController::TplController()
  {
  }

  ErrorCode ToolManagerNode::TplController::perform()
  {
    return ErrorCode::Success;
  }
}  // namespace tool_manager