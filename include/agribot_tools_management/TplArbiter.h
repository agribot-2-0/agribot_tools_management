/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <string>
#include <tuple>

namespace tool_manager
{
enum class ControlSource
{
  Manual = 0,
  Navigation = 1,
  None = 2
};
class TplArbiter
{
 private:
  const struct
  {
    std::string front_manual = "/agribot/control/tool/front_manual";
    std::string back_manual = "/agribot/control/tool/back_manual";
    std::string front_navigation = "/agribot/control/tool/front_navigation";
    std::string back_navigation = "/agribot/control/tool/back_navigation";
    std::string front_destinaton = "/agribot/control/tool/front";
    std::string back_destinaton = "/agribot/control/tool/back";
  } topic_names_;
  ControlSource source_;

 public:
  TplArbiter() : source_(ControlSource::None){};
  void setTplControlSource(ControlSource const src);
  ControlSource getTplControlSource() const;
  std::tuple<std::string, std::string> getFrtBckSourceTopics() const;
  std::tuple<std::string, std::string> getFrtBckDestinationTopics() const;
};
}  // namespace tool_manager