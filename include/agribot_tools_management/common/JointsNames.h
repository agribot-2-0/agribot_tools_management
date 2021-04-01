/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#pragma once

#include <string>
namespace tool_manager
{
// The below joint names should be the same as those specified in .yaml configs and /robot_description
inline const struct
{
  std::string front_tpl_lower_hitches_revolute = "front_tpl_lower_hitches_revolute_joint";
  std::string back_tpl_lower_hitches_revolute = "back_tpl_lower_hitches_revolute_joint";
  std::string front_tpl_upper_hitch_prismatic = "front_tpl_upper_hitch_prismatic_joint";
  std::string back_tpl_upper_hitch_prismatic = "back_tpl_upper_hitch_prismatic_joint";
} joint_names;
}  // namespace tool_manager
