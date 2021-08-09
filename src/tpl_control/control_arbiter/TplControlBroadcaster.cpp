/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#include "agribot_tools_management/tpl_control/control_arbiter/TplControlBroadcaster.h"

namespace tool_manager
{
TplControlBroadcaster::TplControlBroadcaster()
{
  tpl_config_[ConfigIndex::FrontManual] = TplConfig();
  tpl_config_[ConfigIndex::BackManual] = TplConfig();
  tpl_config_[ConfigIndex::FrontNavigation] = TplConfig();
  tpl_config_[ConfigIndex::BackNavigation] = TplConfig();
}

void TplControlBroadcaster::setConfig(ConfigIndex const idx, TplConfig const conf)
{
  tpl_config_[idx] = conf;
}

std::tuple<TplConfig, TplConfig> TplControlBroadcaster::getConfigsFrontBack(ControlSource src) const
{
  switch (src)
  {
    case ControlSource::Manual:
      return { tpl_config_.at(ConfigIndex::FrontManual), tpl_config_.at(ConfigIndex::BackManual) };
    case ControlSource::Navigation:
      return { tpl_config_.at(ConfigIndex::FrontNavigation), tpl_config_.at(ConfigIndex::BackNavigation) };
    default:
      auto default_tpl_config = TplConfig();
      default_tpl_config.tpl_angle = 0.8;
      default_tpl_config.tpl_height = 0.05;
      return { default_tpl_config, default_tpl_config };
  }
}
}  // namespace tool_manager
