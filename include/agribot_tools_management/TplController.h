/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <unordered_map>
#include <tuple>
#include "agribot_tools_management/ControlSource.h"

namespace tool_manager
{
enum class ConfigIndex
{
  FrontManual = 0,
  BackManual = 1,
  FrontNavi = 2,
  BackNavi = 3
};
class HashConf
{
 public:
  std::size_t operator()(const ConfigIndex& idx) const
  {
    return static_cast<std::size_t>(idx);
  }
};
struct TplConfig
{
  double tpl_angle = 0.0;
  double tpl_height = 0.0;
  double red_valve_flow = 0.0;
  double green_valve_flow = 0.0;
  double blue_valve_flow = 0.0;
  double pto_rpm = 0.0;

  void operator=(const TplConfig& cfg)
  {
    tpl_angle = cfg.tpl_angle;
    tpl_height = cfg.tpl_height;
    red_valve_flow = cfg.red_valve_flow;
    green_valve_flow = cfg.green_valve_flow;
    blue_valve_flow = cfg.blue_valve_flow;
    pto_rpm = cfg.pto_rpm;
  }
};

class TplController
{
 private:
  std::unordered_map<ConfigIndex, TplConfig, HashConf> tpl_config_;

 public:
  TplController();
  void setConfig(ConfigIndex const idx, TplConfig const conf);
  const std::tuple<TplConfig, TplConfig> getConfigsFrtBck(ControlSource src) const;
};
}  // namespace tool_manager
