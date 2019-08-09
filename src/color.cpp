#include "makemeglow/color.h"

#include <algorithm>
#include <array>
#include <limits>

namespace glow {

namespace {

constexpr float FLOAT_COLOR_CHANNEL_MIN = 0.f;
constexpr float FLOAT_COLOR_CHANNEL_MAX = 255.f;
// scale factor for 8.8 fixed-point nums
constexpr uint32_t SCALE_FACTOR = 256;

} // namespace

float clampChannel(float f)
{
    return std::clamp(f, FLOAT_COLOR_CHANNEL_MIN, FLOAT_COLOR_CHANNEL_MAX);
}

Color blendColors(Color bg, Color fg)
{
    // TODO fix bg alpha
    // treat nums as 8.8 fixed-point
    const uint32_t alpha = fg.a();
    const uint32_t invAlpha = (255 - alpha);
    return {
        static_cast<uint8_t>((alpha * fg.r() + invAlpha * bg.r()) / SCALE_FACTOR),
        static_cast<uint8_t>((alpha * fg.g() + invAlpha * bg.g()) / SCALE_FACTOR),
        static_cast<uint8_t>((alpha * fg.b() + invAlpha * bg.b()) / SCALE_FACTOR),
        static_cast<uint8_t>((alpha * fg.a() + invAlpha * bg.a()) / SCALE_FACTOR)
    };
}

} // namespace glow
