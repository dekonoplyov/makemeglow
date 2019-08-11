#include "glow_buffer.h"

#include <stdexcept>

namespace glow {

namespace {

inline Color applyIntensity(Color textColor, float intensity)
{
    return {textColor.r(), textColor.g(), textColor.b(), static_cast<uint8_t>(textColor.a() * intensity)};
}

} // namespace

ColorBuffer glowBuffer(
    const IntensityBuffer& buffer,
    const std::vector<float>& weights,
    Color textColor,
    Color backgroundColor)
{
    if (weights.empty()) {
        throw std::runtime_error{"Can't glow buffer with empty weights"};
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
            const auto intensity = std::max(clampChannel(res), static_cast<float>(buffer(x, y))) / 255.f;
            verticalPass(x, y) = blendColors(backgroundColor, applyIntensity(textColor, intensity));
        }
    }

    return verticalPass;
}

} // namespace glow
