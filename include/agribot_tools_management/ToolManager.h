/*
* Copyright (C) 2020 Agribot. All rights reserved.
*/

#pragma once

#include <cstdio>
#include <cstdlib>
#include <vector>

class ToolManager
{
  private:
    //vector<uint8_t> front_tags_id_vector_;
    //vector<uint8_t> back_tags_id_vector_;
    uint8_t state_;
  public:
    ToolManager();
    bool setState(uint8_t state);
};