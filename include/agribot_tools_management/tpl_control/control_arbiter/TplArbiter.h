/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <tuple>

#include "agribot_tools_management/common/ControlSource.h"
#include "agribot_tools_management/common/ConfigIndex.h"

namespace tool_manager
{
constexpr float sec_to_ms = 0.001;
class TplArbiter
{
 private:
  ControlSource source_;
  int timeout_val_;
  std::unordered_map<ConfigIndex, int, HashConf> timeout_;

  void incrementTimeoutCnts();
 public:
  TplArbiter();
  void setTimeout(int timeout, int loop_rate);
  void setTplControlSource(ControlSource const src);
  ControlSource getTplControlSource() const;
  bool sourceSelected() const;
  void monitorResetTimeoutCnt(ConfigIndex const idx);
  void monitorResetTimeoutCnts();
  bool timeoutsExceeded();
  std::tuple<std::string, std::string> getFrontBackSourceTopics(ControlSource const src) const;
  std::tuple<std::string, std::string> getFrontBackDestinationTopics() const;
};
}  // namespace tool_manager
