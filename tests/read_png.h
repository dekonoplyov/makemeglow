#pragma once

#include "makemeglow/color_buffer.h"

namespace glow {

/*
 * For tests purposes
 * supports only 8bit depth and PNG_COLOR_TYPE_GRAY
 * It's also not very efficient
 */
IntensityBuffer readPng(const std::string& filename);

} // namespace glow
