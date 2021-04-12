/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#include "agribot_tools_management/tpl_control/control_arbiter/TplArbiter.h"
#include "agribot_tools_management/common/TplControlTopicName.h"

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
  timeout_val_ = (timeout * loop_rate) * sec_to_ms;
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
    default:
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
    static_cast<void>(timeout_value);  // suppress [-Wunused-variable]
    TplArbiter::monitorResetTimeoutCnt(timeout_index);
  }
}

bool TplArbiter::timeoutsExceeded()
{
  incrementTimeoutCnts();

  for (const auto& [timeout_index, timeout_value] : timeout_)
  {
    static_cast<void>(timeout_index);  // suppress [-Wunused-variable]
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
      return { tpl_control_topic_name.front_manual_setpoint, tpl_control_topic_name.back_manual_setpoint };
    case ControlSource::Navigation:
      return { tpl_control_topic_name.front_navigation_setpoint, tpl_control_topic_name.back_navigation_setpoint };
    default:
      return { "", "" };
  }
}

std::tuple<std::string, std::string> TplArbiter::getFrontBackDestinationTopics() const
{
  return { tpl_control_topic_name.front_setpoint, tpl_control_topic_name.back_setpoint };
}
}  // namespace tool_manager
