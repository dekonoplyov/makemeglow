#include "makemeglow/glow.h"

#include "font_rasterizer.h"
#include "gauss.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>

namespace glow {

namespace {

ColorBuffer gaussianBlur(
    const IntensityBuffer& buffer,
    const std::vector<float>& weights,
    Color textColor,
    Color backgroundColor)
{
    // TODO error message
    if (weights.empty()) {
        throw std::runtime_error{"Can't work with empty weights"};
    }

    const int yLimit = static_cast<int>(buffer.height());
    const int xLimit = static_cast<int>(buffer.width());
    const int radius = static_cast<int>(weights.size());

    IntensityBuffer horizontalPass{buffer.width(), buffer.height()};
    for (int y = 0; y < yLimit; ++y) {
        for (int x = 0; x < xLimit; ++x) {
            float res = static_cast<float>(buffer(x, y)) * weights[0];
            for (int i = 1; i < radius; ++i) {
                if (x + i < xLimit) {
                    res += static_cast<float>(buffer(x + i, y)) * weights[i];
                }
                if (x - i >= 0) {
                    res += static_cast<float>(buffer(x - i, y)) * weights[i];
                }
            }
            horizontalPass(x, y) = static_cast<uint8_t>(clampChannel(res));
        }
    }

    ColorBuffer verticalPass{buffer.width(), buffer.height()};
    for (int y = 0; y < yLimit; ++y) {
        for (int x = 0; x < xLimit; ++x) {
            float res = static_cast<float>(horizontalPass(x, y)) * weights[0];
            for (int i = 1; i < radius; ++i) {
                if (y + i < yLimit) {
                    res += static_cast<float>(horizontalPass(x, y + i)) * weights[i];
                }
                if (y - i >= 0) {
                    res += static_cast<float>(horizontalPass(x, y - i)) * weights[i];
                }
            }

            const auto intensity = std::max(static_cast<uint8_t>(clampChannel(res)), buffer(x, y)) / 255.f;
            Color c{textColor.r(), textColor.g(), textColor.b(), static_cast<uint8_t>(textColor.a() * intensity)};
            verticalPass(x, y) = blendColors(backgroundColor, c);
        }
    }

    return verticalPass;
}

} // namespace

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
        return gaussianBlur(intensityBuffer, /* weights */ {1.f}, textColor, backgroundColor);
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
        return gaussianBlur(intensityBuffer,
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
        return gaussianBlur(intensityBuffer,
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
