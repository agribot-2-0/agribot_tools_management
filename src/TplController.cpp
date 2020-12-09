/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/TplController.h"

namespace tool_manager
{
TplController::TplController()
{
  tpl_config_[ConfigIndex::FrontManual] = TplConfig();
  tpl_config_[ConfigIndex::BackManual] = TplConfig();
  tpl_config_[ConfigIndex::FrontNavi] = TplConfig();
  tpl_config_[ConfigIndex::BackNavi] = TplConfig();
}

void TplController::setConfig(ConfigIndex const idx, TplConfig const conf)
{
  tpl_config_[idx] = conf;
}

std::tuple<TplConfig, TplConfig> TplController::getConfigsFrtBck(ControlSource src) const
{
  switch (src)
  {
    case ControlSource::Manual:
      return { tpl_config_.at(ConfigIndex::FrontManual), tpl_config_.at(ConfigIndex::BackManual) };
    case ControlSource::Navigation:
      return { tpl_config_.at(ConfigIndex::FrontNavi), tpl_config_.at(ConfigIndex::BackNavi) };
    default:
      return { TplConfig(), TplConfig() };
  }
}
}  // namespace tool_manager
