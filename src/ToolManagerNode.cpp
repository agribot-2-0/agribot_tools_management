/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

ToolManagerNode::ToolManagerNode(int argc, char** argv)
{
  ros::init(argc, argv, "agribot_tools_management_node");
  node_handle_ = std::make_shared<ros::NodeHandle>();
  tool_state_service_ = node_handle_->advertiseService("agribot/tool_management_state", &ToolManagerNode::state_request_service, this);
}

bool ToolManagerNode::state_request_service(agribot_msgs::ToolState::Request& req,   // NOLINT(runtime/references)
                                            agribot_msgs::ToolState::Response& res)  // NOLINT(runtime/references)
{
  bool ret = false;
  switch (req.requested_state)
  {
    case req.IDLE:
      // TBD: tool manager idle state handle
      break;
    case req.SCAN:
      // tool manager scan tasks handle
      tool_manager_.setState(ToolManagerState::scan);
      ret = true;
      break;
    case req.TOOLS_MANUAL:
      // TBD: tool manager manual tools control handle
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