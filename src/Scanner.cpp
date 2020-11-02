/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/Scanner.h"

namespace tool_manager
{
Scanner::Scanner()
{
  pwr_[Antenna::Front] = antenna_power_info_.min_ant_pwr;
  pwr_[Antenna::Back] = antenna_power_info_.min_ant_pwr;
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

bool Scanner::entirePowerRangeInspected()
{
  if (pwr_change_attempts_ >= max_number_of_iterations_)
  {
    return true;
  }
  return false;
}

void Scanner::increaseAntennaPower()
{
  if (pwr_[ant_] < antenna_power_info_.max_ant_pwr)
  {
    pwr_[ant_] += antenna_power_info_.ant_pwr_resolution;
  }
}

void Scanner::decreaseAntennaPower()
{
  if (pwr_[ant_] > antenna_power_info_.min_ant_pwr)
  {
    pwr_[ant_] -= antenna_power_info_.ant_pwr_resolution;
  }
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
  if (tags_cnt_ == 0)  // increase power, if no tags detected
  {
    increaseAntennaPower();
  }
  if (tags_cnt_ > 1)
  {
    decreaseAntennaPower();  // decrease power, if no more than one tag detected
  }
}

}  // namespace tool_manager
