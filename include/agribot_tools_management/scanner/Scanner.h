/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <unordered_map>

namespace tool_manager
{
enum class Antenna
{
  Front = 1,
  Back = 2
};
class HashAnt
{
 public:
  std::size_t operator()(const Antenna& ant) const
  {
    return static_cast<std::size_t>(ant);
  }
};
class Scanner
{
 private:
  const std::size_t max_number_of_iterations_ = 128;
  const struct
  {
    double ant_pwr_resolution;
    double min_ant_pwr;
    double max_ant_pwr;
  } antenna_power_info_{ 0.25, 17.0, 33.0 };

  Antenna ant_;
  std::size_t pwr_change_attempts_;
  std::unordered_map<Antenna, double, HashAnt> pwr_;
  uint8_t tags_cnt_;

 public:
  Scanner();
  void setAnt(Antenna sel_ant);
  Antenna getAnt() const;
  bool entirePowerRangeInspected() const;
  void increaseAntennaPower();
  void decreaseAntennaPower();
  void setTagsCnt(std::size_t number_of_tags);
  bool scanCorrect() const;
  double adjustPower();
};
}  // namespace tool_manager
