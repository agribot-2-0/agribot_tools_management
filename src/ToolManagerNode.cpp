/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

namespace tool_manager
{
  ToolManagerNode::ToolManagerNode(int argc, char** argv)
  {
    ros::init(argc, argv, "agribot_tools_management_node");
    node_handle_ = std::make_shared<ros::NodeHandle>();
    tool_state_srv_ =
        node_handle_->advertiseService("agribot/tool_management_state", &ToolManagerNode::state_request_service, this);
  }

  bool ToolManagerNode::checkErr(ErrorCode err,
                                 agribot_msgs::ToolManagerState::Response& res)  // NOLINT(runtime/references)
  {
    if (err == ErrorCode::Success)
    {
      res.result = res.SUCCESS;
      return true;
    }
    else
    {
      res.result = res.FAIL;
      ROS_ERROR_STREAM("[TOOL MANAGEMENT] Request failed with error code: " << static_cast<int>(err));
      return false;
    }
  }

  bool
  ToolManagerNode::state_request_service(agribot_msgs::ToolManagerState::Request& req,   // NOLINT(runtime/references)
                                         agribot_msgs::ToolManagerState::Response& res)  // NOLINT(runtime/references)
  {
    ErrorCode ret = ErrorCode::Success;
    switch (req.requested_state)
    {
      case req.IDLE:
        ret = ErrorCode::Success;
        break;
      case req.SCAN_FRONT:
        scan_handler_.setAnt(Antenna::Front);
        ret = scan_handler_.perform();
        break;
      case req.SCAN_BACK:
        scan_handler_.setAnt(Antenna::Back);
        ret = scan_handler_.perform();
        break;
      case req.SET_TPL:
        ret = tlp_control_handler_.perform();
        break;
    }
    return checkErr(ret, res);
  }

  void ToolManagerNode::run()
  {
    ros::Rate loop_rate(1);
    while (ros::ok())
    {
      ros::spinOnce();
      loop_rate.sleep();
    }
  }

  ToolManagerNode::~ToolManagerNode()
  {
  }

}  // namespace tool_manager