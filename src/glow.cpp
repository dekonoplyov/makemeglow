#include "makemeglow/glow.h"

#include "gauss_kernel.h"
#include "glow_buffer.h"
#include "makemeglow/font_rasterizer.h"

#include <algorithm>
#include <array>
#include <cstdint>

namespace glow {

class Rasterizer : public RasterizerInterface {
public:
    Rasterizer(const std::string& font)
        : fontRasterizer_{makeFontRasterizer(font)}
    {
    }

    ColorBuffer rasterize(
        const std::string& text,
        size_t pixelSize,
        Color textColor,
        Color backgroundColor) override
    {
        const auto intensityBuffer = fontRasterizer_->rasterize(text, pixelSize, /* margin */ 0);
        return glowBuffer(intensityBuffer, /* weights */ {1.f}, textColor, backgroundColor);
    }

    ColorBuffer rasterize(
        const std::string& text,
        size_t pixelSize,
        Color textColor,
        Color backgroundColor,
        GlowParams glowParams) override
    {
        const auto intensityBuffer = fontRasterizer_->rasterize(text, pixelSize,
            /* margin */ glowParams.kernelSize / 2);
        return glowBuffer(intensityBuffer,
            gauss1dKernel(glowParams.kernelSize, glowParams.sigma),
            textColor, backgroundColor);
    }

    ColorBuffer rasterize(
        const std::string& text,
        size_t pixelSize,
        Color textColor,
        Color backgroundColor,
        const std::vector<float>& weights) override
    {
        const auto intensityBuffer = fontRasterizer_->rasterize(text, pixelSize,
            /* margin */ weights.size() - 1);
        return glowBuffer(intensityBuffer,
            weights,
            textColor, backgroundColor);
    }

private:
    FontRasterizerPtr fontRasterizer_;
};

RasterizerPtr makeRasterizer(const std::string& font)
{
    return std::make_unique<Rasterizer>(font);
}

} // namespace glow
