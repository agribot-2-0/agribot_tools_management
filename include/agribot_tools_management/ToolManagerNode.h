/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <ros/ros.h>
#include <ros/console.h>

#include <agribot_msgs/Tags.h>
#include <agribot_msgs/ToolManagerState.h>
#include <agribot_msgs/RfidPower.h>
#include "agribot_tools_management/ToolManager.h"
#include "agribot_tools_management/ToolManagerTask.h"

namespace ToolManager
{
  enum class Antennas
  {
    Front = 1,
    Back = 2
  };
  class ToolManagerNode
  {
  private:
    class Scanner: public ToolManagerTask
    {
    public:
      Scanner();
      ErrorCode perform();
    } scan_handler_;
    class TplController: public ToolManagerTask
    {
    public:
      TplController();
      ErrorCode perform();
    } tlp_control_handler_;

    std::shared_ptr<ros::NodeHandle> node_handle_;
    ros::ServiceServer tool_state_srv_;
    ros::ServiceClient tags_srv_client_;
    ros::ServiceClient rfid_power_srv_client_;

    bool state_request_service(agribot_msgs::ToolManagerState::Request& req,    // NOLINT(runtime/references)
                               agribot_msgs::ToolManagerState::Response& res);  // NOLINT(runtime/references)
  public:
    ToolManagerNode(int argc, char** argv);
    ~ToolManagerNode();
    void run();
  };
}  // namespace ToolManager