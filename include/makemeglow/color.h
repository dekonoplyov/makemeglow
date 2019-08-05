#pragma once

#include <cstdint>

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

    Color operator*(float m);
    Color operator+(Color c);

private:
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
    uint8_t a_;
};

} // namespace glow
