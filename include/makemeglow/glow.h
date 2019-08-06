#pragma once

#include "makemeglow/color_buffer.h"

namespace glow {

ColorBuffer rasterize(
    const std::string& text,
    const std::string& font,
    size_t pixelSize,
    Color textColor,
    Color BackgroundColor);

} // namespace glow
