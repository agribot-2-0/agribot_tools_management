/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

ToolManagerNode::ToolManagerNode()
{
}

ToolManagerNode::ToolManagerNode(ros::NodeHandle* nh) : node_handle_(nh)
{
  tool_state_service_ = node_handle_->advertiseService("agribot/tool_management_state", &ToolManagerNode::state_request_service, this);
}

bool ToolManagerNode::state_request_service(agribot_msgs::ToolState::Request& req,   // NOLINT(runtime/references)
                                            agribot_msgs::ToolState::Response& res)  // NOLINT(runtime/references)
{
  bool ret = false;
  switch (req.requested_state)
  {
    case req.IDLE:
      // tool manager idle state handle
      break;
    case req.SCAN:
      // tool manager scan tasks handle
      // Perform
      tool_manager_.setState(req.SCAN);
      ret = false;
      break;
    case req.TOOLS_MANUAL:
      // tool manager manual tools control handle
      break;
  }
  return ret;
}

void ToolManagerNode::run()
{
  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
}

ToolManagerNode::~ToolManagerNode()
{
}