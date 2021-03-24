/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <string>

namespace tool_manager
{
inline const struct
{
  std::string front_manual_setpoint = "/agribot/control/tool/front/manual";
  std::string back_manual_setpoint = "/agribot/control/tool/back/manual";
  std::string front_navigation_setpoint = "/agribot/control/tool/front/navigation";
  std::string back_navigation_setpoint = "/agribot/control/tool/back/navigation";
  std::string front_setpoint = "/agribot/control/tool/front";
  std::string back_setpoint = "/agribot/control/tool/back";
  std::string front_control = "/agribot/control/tool/front/valves";
  std::string back_control = "/agribot/control/tool/back/valves";
  std::string front_feedback = "/agribot/encoder/tool/front";
  std::string back_feedback = "/agribot/encoder/tool/back";
} tpl_control_topic_name;
}