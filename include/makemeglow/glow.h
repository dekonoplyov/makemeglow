#pragma once

#include "makemeglow/buffer.h"

#include <memory>

namespace glow {

struct GlowParams {
    size_t kernelSize;
    float sigma;
};

class RasterizerInterface {
public:
    virtual ~RasterizerInterface() {};

    virtual ColorBuffer rasterize(
        const std::string& text,
        size_t pixelSize,
        Color textColor,
        Color BackgroundColor) = 0;

    virtual ColorBuffer rasterize(
        const std::string& text,
        size_t pixelSize,
        Color textColor,
        Color BackgroundColor,
        GlowParams glowParams) = 0;

    virtual ColorBuffer rasterize(
        const std::string& text,
        size_t pixelSize,
        Color textColor,
        Color BackgroundColor,
        const std::vector<float>& weights) = 0;
};

using RasterizerPtr = std::unique_ptr<RasterizerInterface>;

RasterizerPtr makeRasterizer(const std::string& font);

} // namespace glow
