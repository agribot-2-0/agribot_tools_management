/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#pragma once
#include <agribot_msgs/AgribotTool.h>
#include <agribot_common/TplConfig.h>

namespace tool_manager
{
namespace msg_struct_transform_functions
{
inline tool_manager::TplConfig toolMsgToTplConfig(const agribot_msgs::AgribotTool::ConstPtr& tool_msg)
{
  return { tool_msg->tpl_angle,        tool_msg->tpl_height,      tool_msg->red_valve_flow,
           tool_msg->green_valve_flow, tool_msg->blue_valve_flow, tool_msg->pto_rpm };
}

inline void tplConfigToToolMsg(const tool_manager::TplConfig& config,
                               agribot_msgs::AgribotTool& tool_msg)  // NOLINT(runtime/references)
{
  tool_msg.tpl_angle = config.tpl_angle;
  tool_msg.tpl_height = config.tpl_height;
  tool_msg.red_valve_flow = config.red_valve_flow;
  tool_msg.green_valve_flow = config.green_valve_flow;
  tool_msg.blue_valve_flow = config.blue_valve_flow;
  tool_msg.pto_rpm = config.pto_rpm;
}
}  // namespace msg_struct_transform_functions
}  // namespace tool_manager
