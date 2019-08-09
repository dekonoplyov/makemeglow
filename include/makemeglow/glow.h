#pragma once

#include "makemeglow/buffer.h"

#include <memory>

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

class Rasterizer {
public:
    Rasterizer(const std::string& font);
    ~Rasterizer();

    ColorBuffer rasterize(
        const std::string& text,
        size_t pixelSize,
        Color textColor,
        Color BackgroundColor);

    ColorBuffer rasterize(
        const std::string& text,
        size_t pixelSize,
        Color textColor,
        Color BackgroundColor,
        GlowParams glowParams);

    ColorBuffer rasterize(
        const std::string& text,
        size_t pixelSize,
        Color textColor,
        Color BackgroundColor,
        const std::vector<float>& weights);

private:
    class RasterizerImpl;
    std::unique_ptr<RasterizerImpl> impl_;
};

} // namespace glow
