#include "makemeglow/glow.h"
#include "makemeglow/font_rasterizer.h"
#include "gauss.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
#include <iostream>

namespace glow {

namespace {

ColorBuffer gaussianBlur(
    const IntensityBuffer& buffer,
    const std::vector<float>& weights,
    Color textColor,
    Color backgroundColor)
{
    const int yLimit = static_cast<int>(buffer.height());
    const int xLimit = static_cast<int>(buffer.width());
    const int radius = static_cast<int>(weights.size());

    IntensityBuffer horizontalPass{buffer.width(), buffer.height()};
    for (int y = 0; y < yLimit; ++y) {
        for (int x = 0; x < xLimit; ++x) {
            float res = static_cast<float>(buffer.at(x, y)) * weights[0];
            for (int i = 1; i < radius; ++i) {
                if (x + i < xLimit) {
                    res += static_cast<float>(buffer.at(x + i, y)) * weights[i];
                }
                if (x - i >= 0) {
                    res += static_cast<float>(buffer.at(x - i, y)) * weights[i];
                }
            }
            horizontalPass.at(x, y) = static_cast<uint8_t>(clampChannel(res));
        }
    }

    ColorBuffer verticalPass{buffer.width(), buffer.height()};
    for (int y = 0; y < yLimit; ++y) {
        for (int x = 0; x < xLimit; ++x) {
            float res = static_cast<float>(horizontalPass.at(x, y)) * weights[0];
            for (int i = 1; i < radius; ++i) {
                if (y + i < yLimit) {
                    res += static_cast<float>(horizontalPass.at(x, y + i)) * weights[i];
                }
                if (y - i >= 0) {
                    res += static_cast<float>(horizontalPass.at(x, y - i)) * weights[i];
                }
            }

            const auto intensity = std::max(static_cast<uint8_t>(clampChannel(res)), buffer.at(x, y)) / 255.f;
            Color c{textColor.r(), textColor.g(), textColor.b(), static_cast<uint8_t>(textColor.a() * intensity)};
            verticalPass.at(x, y) = blendColors(backgroundColor, c);
        }
    }

    return verticalPass;
}

} // namespace

ColorBuffer rasterize(
    const std::string& text,
    const std::string& font,
    size_t pixelSize,
    Color textColor,
    Color BackgroundColor)
{
    // weights to get margin
    const auto weights = createGauss1dKernel();
    FontRasterizer rasterizer{font};
    const auto intensityBuffer = rasterizer.rasterize(text, pixelSize, /*margin*/ weights.size());
    return gaussianBlur(intensityBuffer, weights, textColor, BackgroundColor);
}

} // namespace glow
