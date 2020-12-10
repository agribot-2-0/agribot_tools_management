/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

enum class ConfigIndex
{
  FrontManual = 0,
  BackManual = 1,
  FrontNavi = 2,
  BackNavi = 3
};
class HashConf
{
 public:
  std::size_t operator()(const ConfigIndex& idx) const
  {
    return static_cast<std::size_t>(idx);
  }
};
