/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/ToolManagerNode.h"

namespace tool_manager
{
  Scanner::Scanner()
  {
    pwr_[Antenna::Front] = antenna_power_info.MinAntPower;
    pwr_[Antenna::Back] = antenna_power_info.MinAntPower;
  }

  void Scanner::setAnt(uint8_t sel_ant)
  {
    pwr_change_attempts_ = 0;  // new antenna, new attempts cnt
    ant_ = static_cast<Antenna>(sel_ant);
  }

  uint8_t Scanner::getAnt()
  {
    return static_cast<uint8_t>(ant_);
  }

  void Scanner::increaseAntennaPower()
  {
    if (pwr_[ant_] < antenna_power_info.MaxAntPower)
    {
      pwr_[ant_] += antenna_power_info.AntPwrResolution;
    }
  }

  void Scanner::decreaseAntennaPower()
  {
    if (pwr_[ant_] > antenna_power_info.MinAntPower)
    {
      pwr_[ant_] -= antenna_power_info.AntPwrResolution;
    }
  }

  bool Scanner::entirePowerRangeInspected()
  {
    if (pwr_change_attempts_ >= max_number_of_iterations)
    {
      return false;
    }
    return true;
  }

  void Scanner::setTagsCnt(std::size_t number_of_tags)
  {
    tags_cnt_ = number_of_tags;
  }

  bool Scanner::scanCorrect()
  {
    return tags_cnt_ == 1 ? true : false;
  }

  double Scanner::adjustPower()
  {
    pwr_change_attempts_++;
    switch (tags_cnt_)
    {
      case 0:
        increaseAntennaPower();
        break;
      case 2:
        decreaseAntennaPower();
        break;
    }
  }

}  // namespace tool_manager