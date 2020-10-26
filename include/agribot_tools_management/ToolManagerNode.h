/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <ros/ros.h>
#include <ros/console.h>

#include <agribot_msgs/Tags.h>
#include <agribot_msgs/ToolManagerState.h>
#include <agribot_msgs/RfidPower.h>
#include "agribot_tools_management/ToolManagerTask.h"
#include <vector>

namespace tool_manager
{
  enum class Antenna
  {
    Front = 1,
    Back = 2
  };

  // TODO: implement hash class to be able to create unordered map with custom hash values
  class HashAnt
  {
  public:
    std::size_t operator()(const Antenna& a) const
    {
      return static_cast<std::size_t>(a);
    }
  };

  class ToolManagerNode
  {
  private:
    class Scanner : public ToolManagerTask
    {
    private:
      const std::size_t max_number_of_iterations = 128;
      const struct
      {
        double AntPwrResolution;
        double MinAntPower;
        double MaxAntPower;
      } antenna_power_info{ 0.25, 17.0, 33.0 };
      Antenna ant_;
      std::size_t pwr_change_attempts_;
      std::map<double, int> pwr_;
      std::vector<uint8_t> detected_tags_;

    public:
      Scanner();
      void increase_antenna_power();
      void setAnt(Antenna sel_ant);
      void decrease_antenna_power();
      bool power_range_inspected();
      bool scan();
      std::vector<uint8_t> getDetectedTags();
      ErrorCode perform();
    } scan_handler_;

    class TplController : public ToolManagerTask
    {
    public:
      TplController();
      ErrorCode perform();
    } tpl_control_handler_;

    std::shared_ptr<ros::NodeHandle> node_handle_;
    ros::ServiceServer tool_state_srv_;
    ros::ServiceClient tags_srv_client_;
    ros::ServiceClient rfid_power_srv_client_;

    bool checkErr(ErrorCode err, agribot_msgs::ToolManagerState::Response& res);  // NOLINT(runtime/references)
    bool state_request_service(agribot_msgs::ToolManagerState::Request& req,      // NOLINT(runtime/references)
                               agribot_msgs::ToolManagerState::Response& res);    // NOLINT(runtime/references)
  public:
    ToolManagerNode(int argc, char** argv);
    ~ToolManagerNode();
    void run();
  };
}  // namespace tool_manager