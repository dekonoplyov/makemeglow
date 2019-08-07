#pragma once

#include "makemeglow/color_buffer.h"

namespace glow {

void writePng(const std::string& filename, const ColorBuffer& buffer);
void writePng(const std::string& filename, const IntensityBuffer& buffer);

} // namespace glow
