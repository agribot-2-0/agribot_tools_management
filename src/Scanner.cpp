/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

namespace tool_manager
{
  ToolManagerNode::Scanner::Scanner()
  {
  }

  void ToolManagerNode::Scanner::setAnt(Antenna sel_ant)
  {
    ant_ = sel_ant;
  }

  ErrorCode ToolManagerNode::Scanner::perform()
  {
    return ErrorCode::Success;
  }
}  // namespace tool_manager