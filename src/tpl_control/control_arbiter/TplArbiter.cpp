/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/tpl_control/control_arbiter/TplArbiter.h"

namespace tool_manager
{
TplArbiter::TplArbiter() : source_(ControlSource::None)
{
  timeout_[ConfigIndex::FrontManual] = 0;
  timeout_[ConfigIndex::BackManual] = 0;
  timeout_[ConfigIndex::FrontNavigation] = 0;
  timeout_[ConfigIndex::FrontNavigation] = 0;
}

void TplArbiter::setTimeout(int timeout, int loop_rate)
{
  timeout_val_ = (timeout*loop_rate)*sec_to_ms;
}

void TplArbiter::setTplControlSource(ControlSource const src)
{
  source_ = src;
}

ControlSource TplArbiter::getTplControlSource() const
{
  return source_;
}

bool TplArbiter::sourceSelected() const
{
  return source_ != ControlSource::None;
}

void TplArbiter::incrementTimeoutCnts()
{
  switch (source_)
  {
    case ControlSource::Manual:
      timeout_[ConfigIndex::FrontManual] += 1;
      timeout_[ConfigIndex::BackManual] += 1;
      break;
    case ControlSource::Navigation:
      timeout_[ConfigIndex::FrontNavigation] += 1;
      timeout_[ConfigIndex::BackNavigation] += 1;
      break;
  }
}

void TplArbiter::monitorResetTimeoutCnt(ConfigIndex const idx)
{
  timeout_[idx] = 0;
}

void TplArbiter::monitorResetTimeoutCnts()
{
  for (const auto& [timeout_index, timeout_value] : timeout_)
  {
    TplArbiter::monitorResetTimeoutCnt(timeout_index);
  }
}

bool TplArbiter::timeoutsExceeded()
{
  incrementTimeoutCnts();

  for (const auto& [timeout_index, timeout_value] : timeout_)
  {
    // if there is no controll message on selected topic for more than one second
    if (timeout_value > timeout_val_)
    {
      // and reset counters
      TplArbiter::monitorResetTimeoutCnts();
      return true;
    }
  }
  return false;
}

std::tuple<std::string, std::string> TplArbiter::getFrontBackSourceTopics(ControlSource const src) const
{
  switch (src)
  {
    case ControlSource::Manual:
      return { topic_names_.front_manual, topic_names_.back_manual };
    case ControlSource::Navigation:
      return { topic_names_.front_navigation, topic_names_.back_navigation };
    default:
      return { "", "" };
  }
}

std::tuple<std::string, std::string> TplArbiter::getFrontBackDestinationTopics() const
{
  return { topic_names_.front_destinaton, topic_names_.back_destinaton };
}
}  // namespace tool_manager
