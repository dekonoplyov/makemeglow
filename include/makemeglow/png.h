#pragma once

#include "makemeglow/buffer.h"

namespace glow {

/*
 * These functions are used for testing and example app
 * They support png with 8bit depth, PNG_COLOR_TYPE_GRAY and PNG_COLOR_TYPE_RGB
 * Also, they're not efficient in terms of memory reading/writing
 */
void writePng(const std::string& filename, const ColorBuffer& buffer);
void writePng(const std::string& filename, const IntensityBuffer& buffer);

IntensityBuffer readGrayPng(const std::string& filename);
ColorBuffer readRGBPng(const std::string& filename);

} // namespace glow
