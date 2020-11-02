/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once

namespace tool_manager
{
enum class ErrorCode  // TODO(mwiadoms): Add error codes concerning TPL set case
{
  Success = 0,
  TagsServiceError,
  PowerServiceError
};
}  // namespace tool_manager
