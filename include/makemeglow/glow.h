#pragma once

#include "makemeglow/buffer.h"

namespace glow {

struct GlowParams {
    size_t kernelSize;
    float sigma;
};

ColorBuffer rasterize(
    const std::string& text,
    const std::string& font,
    size_t pixelSize,
    Color textColor,
    Color BackgroundColor,
    GlowParams glowParams = GlowParams{9, 7.f});

} // namespace glow
