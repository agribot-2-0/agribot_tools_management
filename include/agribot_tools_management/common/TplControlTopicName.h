/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
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
  // Topics for setpoints of controllers
  std::string back_tpl_lower_hitches_revolute_control_command = "/agribot/controller/position/"
                                                                "back_tpl_lower_hitches_revolute_control/command";
  std::string back_tpl_upper_hitch_prismatic_control = "/agribot/controller/position/"
                                                                "back_tpl_upper_hitch_prismatic_control/command";
  std::string front_tpl_lower_hitches_revolute_control = "/agribot/controller/position/"
                                                         "front_tpl_lower_hitches_revolute_control/command";
  std::string front_tpl_upper_hitch_prismatic_control = "/agribot/controller/position/"
                                                        "front_tpl_upper_hitch_prismatic_control/command";
} tpl_control_topic_name;
}  // namespace tool_manager
