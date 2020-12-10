/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <tuple>

#include "agribot_tools_management/ControlSource.h"
#include "agribot_tools_management/ConfigIndex.h"

namespace tool_manager
{
constexpr int timeout_val = 100;  // 1s with rate=100

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
  std::unordered_map<ConfigIndex, int, HashConf> timeout_;

  void incrementTimeoutCnts();
 public:
  TplArbiter();
  void setTplControlSource(ControlSource const src);
  ControlSource getTplControlSource() const;
  bool sourceSelected() const;
  void monitorResetTimeoutCnt(ConfigIndex const idx);
  void monitorResetTimeoutCnts();
  bool timeoutsExceeded();
  std::tuple<std::string, std::string> getFrtBckSourceTopics(ControlSource const src) const;
  std::tuple<std::string, std::string> getFrtBckDestinationTopics() const;
};
}  // namespace tool_manager
