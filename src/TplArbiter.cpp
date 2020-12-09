/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include "agribot_tools_management/TplArbiter.h"

namespace tool_manager
{
void TplArbiter::setTplControlSource(ControlSource const src)
{
  source_ = src;
}

ControlSource TplArbiter::getTplControlSource() const
{
  return source_;
}

std::tuple<std::string, std::string> TplArbiter::getFrtBckSourceTopics(ControlSource const src) const
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

std::tuple<std::string, std::string> TplArbiter::getFrtBckDestinationTopics() const
{
  return { topic_names_.front_destinaton, topic_names_.back_destinaton };
}
}  // namespace tool_manager
