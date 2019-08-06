#include "makemeglow/color.h"

#include <algorithm>
#include <array>
#include <limits>

namespace glow {

namespace {

constexpr float FLOAT_COLOR_CHANNEL_MIN = 0.f;
constexpr float FLOAT_COLOR_CHANNEL_MAX = 255.f;

} // namespace

float clampChannel(float f)
{
    return std::clamp(f, FLOAT_COLOR_CHANNEL_MIN, FLOAT_COLOR_CHANNEL_MAX);
}

Color blendColors(Color bg, Color fg)
{
    const uint32_t alpha = fg.a();
    const uint32_t invAlpha = 255 - alpha;
    return {
        static_cast<uint8_t>((alpha * fg.r() + invAlpha * bg.r()) >> 8),
        static_cast<uint8_t>((alpha * fg.g() + invAlpha * bg.g()) >> 8),
        static_cast<uint8_t>((alpha * fg.b() + invAlpha * bg.b()) >> 8),
        0xff
    };
}

} // namespace glow
