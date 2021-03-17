/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

namespace tool_manager
{
class ITplController
{
  virtual void setSetPoint(double set_point) = 0;
  virtual void getControl() = 0;


};
}
