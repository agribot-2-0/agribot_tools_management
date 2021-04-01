/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/TplConfig.h>

#include <unordered_map>
#include <tuple>

#include "agribot_tools_management/common/ControlSource.h"
#include "agribot_tools_management/common/ConfigIndex.h"


namespace tool_manager
{
class TplControlBroadcaster
{
 private:
  std::unordered_map<ConfigIndex, TplConfig, HashConf> tpl_config_;

 public:
  TplControlBroadcaster();
  void setConfig(ConfigIndex const idx, TplConfig const conf);
  std::tuple<TplConfig, TplConfig> getConfigsFrontBack(ControlSource src) const;
};
}  // namespace tool_manager
