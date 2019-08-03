#include "makemeglow/color.h"

#include <algorithm>
#include <limits>

namespace glow {

namespace {

uint8_t clampedSum(uint8_t a, uint8_t b)
{
    return std::clamp(a + b,
        static_cast<int>(std::numeric_limits<uint8_t>::min()),
        static_cast<int>(std::numeric_limits<uint8_t>::max()));
}

uint8_t clampedMultiply(uint8_t a, float m)
{
    return std::clamp(a * m,
        static_cast<float>(std::numeric_limits<uint8_t>::min()),
        static_cast<float>(std::numeric_limits<uint8_t>::max()));
}

} // namespace

Color Color::operator*(float m)
{
    return Color{
        clampedMultiply(r_, m),
        clampedMultiply(g_, m),
        clampedMultiply(b_, m),
        clampedMultiply(a_, m)};
}

Color Color::operator+(Color c)
{
    return Color{
        clampedSum(r_, c.r()),
        clampedSum(g_, c.g()),
        clampedSum(b_, c.g()),
        clampedSum(a_, c.a())};
}

} // namespace glow
