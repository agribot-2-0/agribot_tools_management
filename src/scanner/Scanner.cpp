/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/scanner/Scanner.h"

namespace tool_manager
{
Scanner::Scanner()
{
  pwr_[Antenna::Front] = antenna_power_info_.min_ant_pwr;
  pwr_[Antenna::Back] = antenna_power_info_.min_ant_pwr;
}

void Scanner::setAnt(Antenna sel_ant)
{
  pwr_change_attempts_ = 0;  // new antenna, new attempts cnt
  ant_ = sel_ant;
}

Antenna Scanner::getAnt() const
{
  return ant_;
}

bool Scanner::entirePowerRangeInspected() const
{
  if (pwr_change_attempts_ < max_number_of_iterations_)
  {
    return false;
  }
  return true;
}

void Scanner::increaseAntennaPower()
{
  pwr_[ant_] += antenna_power_info_.ant_pwr_resolution;
  if (pwr_[ant_] > antenna_power_info_.max_ant_pwr)
  {
    pwr_[ant_] = antenna_power_info_.max_ant_pwr;
  }
}

void Scanner::decreaseAntennaPower()
{
  pwr_[ant_] -= antenna_power_info_.ant_pwr_resolution;
  if (pwr_[ant_] < antenna_power_info_.min_ant_pwr)
  {
    pwr_[ant_] = antenna_power_info_.min_ant_pwr;
  }
}

void Scanner::setTagsCnt(std::size_t number_of_tags)
{
  tags_cnt_ = number_of_tags;
}

bool Scanner::scanCorrect() const
{
  return tags_cnt_ == 1;
}

double Scanner::adjustPower()
{
  pwr_change_attempts_++;
  if (tags_cnt_ == 0)  // increase power, if no tags detected
  {
    increaseAntennaPower();
  }
  else if (tags_cnt_ > 1)
  {
    decreaseAntennaPower();  // decrease power, if more than one tag detected
  }
}

}  // namespace tool_manager
