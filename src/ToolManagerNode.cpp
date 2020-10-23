/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

namespace ToolManager
{
  ToolManagerNode::ToolManagerNode(int argc, char** argv)
  {
    ros::init(argc, argv, "agribot_tools_management_node");
    node_handle_ = std::make_shared<ros::NodeHandle>();
    tool_state_srv_ =
        node_handle_->advertiseService("agribot/tool_management_state", &ToolManagerNode::state_request_service, this);
  }

  bool
  ToolManagerNode::state_request_service(agribot_msgs::ToolManagerState::Request& req,   // NOLINT(runtime/references)
                                         agribot_msgs::ToolManagerState::Response& res)  // NOLINT(runtime/references)
  {
    bool ret = false;
    switch (req.requested_state)
      ;
    {
      case req.IDLE:
        ret = true;
        break;
      case req.scan_front:
        scan_handler_.setAnt(Antennas::Front);
        ret = scan_handler_.perform();
        break;
      case req.scan_back:
        scan_handler_.setAnt(Antennas::Back);
        ret = scan_handler_.perform();
        break;
      case req.set_tlp:
        ret = tlp_control_handler_.perform();
        break;
    }
    return ret;
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

}  // namespace ToolManager