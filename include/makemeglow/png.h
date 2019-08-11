#pragma once

#include "makemeglow/buffer.h"

namespace glow {

void writePng(const std::string& filename, const ColorBuffer& buffer);
void writePng(const std::string& filename, const IntensityBuffer& buffer);

/*
 * For testing purposes
 * supports only 8bit depth, PNG_COLOR_TYPE_GRAY and PNG_COLOR_TYPE_RGB
 * It's also not very efficient
 */
IntensityBuffer readGrayPng(const std::string& filename);
ColorBuffer readRGBPng(const std::string& filename);

} // namespace glow
