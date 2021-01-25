/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <unordered_map>
#include <tuple>

#include "agribot_tools_management/common/ControlSource.h"
#include "agribot_tools_management/common/ConfigIndex.h"
#include <agribot_common/TplConfig.h>

namespace tool_manager
{
class TplController
{
 private:
  std::unordered_map<ConfigIndex, TplConfig, HashConf> tpl_config_;

 public:
  TplController();
  void setConfig(ConfigIndex const idx, TplConfig const conf);
  std::tuple<TplConfig, TplConfig> getConfigsFrontBack(ControlSource src) const;
};
}  // namespace tool_manager
