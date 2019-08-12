#pragma once

#include <cstdint>
#include <string>

namespace glow {

class Color {
public:
    // works correct only on little-endian architectures
    Color(uint32_t c = 0)
        : r_{static_cast<uint8_t>((c >> 24) & 0xff)}
        , g_{static_cast<uint8_t>((c >> 16) & 0xff)}
        , b_{static_cast<uint8_t>((c >> 8) & 0xff)}
        , a_{static_cast<uint8_t>(c & 0xff)}
    {
    }

    Color(uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a = 255)
        : r_{r}
        , g_{g}
        , b_{b}
        , a_{a}
    {
    }

    Color(uint8_t c)
        : Color{c, c, c}
    {
    }

    uint8_t r() const
    {
        return r_;
    }

    uint8_t g() const
    {
        return g_;
    }

    uint8_t b() const
    {
        return b_;
    }

    uint8_t a() const
    {
        return a_;
    }

    bool operator==(Color c) const
    {
        return r_ == c.r_ && g_ == c.g_ && b_ == c.b_ && a_ == c.a_;
    }

    bool operator!=(Color c) const
    {
        return !(operator==(c));
    }

private:
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
    uint8_t a_;
};

float clampChannel(float f);

/*
 * https://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending
 */
Color blendColors(Color bg, Color fg);

/*
 * Parse colors in RRGGBB format
 */
Color parseColor(const std::string& s);

} // namespace glow
