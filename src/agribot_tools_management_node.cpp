/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include <ros/ros.h>
#include <ros/console.h>
#include <agribot_msgs/ToolState.h>
#include "agribot_tools_management/ToolManager.h"

ToolManager g_tool_manager;


bool connectedWithMobileApp()
{
  // how to check if agribot is connected with mobile app?
  return true;
}

bool state_request_service(agribot_msgs::ToolState::Request& req,   // NOLINT(runtime/references)
                           agribot_msgs::ToolState::Response& res)  // NOLINT(runtime/references)
{
  bool ret = false;
  switch(req.requested_state)
  {
    case req.IDLE:
      // tool manager idle state handle
      break;
    case req.SCAN:
      // tool manager scan tasks handle
      if(!connectedWithMobileApp)
      {
        g_tool_manager.setState(req.IDLE);
      }
      else
      {
        g_tool_manager.setState(req.SCAN);
        ret = false;
      }
      break;
    case req.TOOLS_MANUAL:
      // tool manager manual tools control handle
      break;
  }
  return ret;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "agribot_tools_management_node");
  ros::NodeHandle n;
  ros::Rate loop_rate(10);
  ros::ServiceServer state_req_service = n.advertiseService("agribot/tool_management_state", state_request_service);

  while (ros::ok())
  {
    if(!connectedWithMobileApp)
    {
      g_tool_manager.setState(req.IDLE);
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
}
