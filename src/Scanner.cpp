/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

namespace tool_manager
{
  ToolManagerNode::Scanner::Scanner()
  {
    pwr_[Antenna::Front] = antenna_power_info.MinAntPower;
    pwr_[Antenna::Back] = antenna_power_info.MinAntPower;
  }

  void ToolManagerNode::Scanner::setAnt(Antenna sel_ant)
  {
    ant_ = sel_ant;
  }

  void ToolManagerNode::Scanner::increase_antenna_power()
  {
    if (pwr_[ant_] < antenna_power_info.MaxAntPower)
    {
      pwr_[ant_] += antenna_power_info.AntPwrResolution;
    }
  }

  void ToolManagerNode::Scanner::decrease_antenna_power()
  {
    if (pwr_[ant_] > antenna_power_info.MinAntPower)
    {
      pwr_[ant_] -= antenna_power_info.AntPwrResolution;
    }
  }

  bool ToolManagerNode::Scanner::entire_power_range_inspected()
  {
    if (pwr_change_attempts_ >= max_number_of_iterations)
    {
      return false;
    }
    return true;
  }

  bool ToolManagerNode::Scanner::scan()
  {
    agribot_msgs::Tags tags_msg;
    tags_msg.request.selected_antenna = static_cast<uint8_t>(ant_);
    if (ToolManagerNode::tags_srv_client_.call(tags_msg))
    {
      for (int i = 0; i < tags_msg.response.ids.size(); i++)
        detected_tags_.push_back(tags_msg.response.ids[i]);
    }
    else
    {
      // Sth went wrong with requesting tags scanu
      return false;
    }
    return true;
  }

  ErrorCode ToolManagerNode::Scanner::perform()
  {
    while (1)
    {
      if (!scan())
      {
        return ErrorCode::RfidServiceError;
      }
      switch (detected_tags_.size())
      {
        case 0:  // No tags detected
          if (entire_power_range_inspected())
          {
            return ErrorCode::NoTagsFound;
          }
          increase_antenna_power();
          break;
        case 1:  // One tag detected
          return ErrorCode::Success;
        case 2:  // Two tag detected
          if (entire_power_range_inspected())
          {
            return ErrorCode::MoreThanOneTagFound;
          }
          decrease_antenna_power();
          break;
      }
    }
  }

  void ToolManagerNode::Scanner::getDetectedTags(std::vector<std::uint16_t>& vDiscoveredTags)
  {
    vDiscoveredTags = detected_tags_;
  }
}  // namespace tool_manager