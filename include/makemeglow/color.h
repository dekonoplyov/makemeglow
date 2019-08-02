#pragma once

#include <cstdint>

namespace glow {

class Color {
public:
    Color(uint32_t c = 0)
        : r_{static_cast<uint8_t>((c >> 24) & 0xff)}
        , g_{static_cast<uint8_t>((c >> 16) & 0xff)}
        , b_{static_cast<uint8_t>((c >> 8) & 0xff)}
        , a_{static_cast<uint8_t>(0xff)}
    {
    }

    Color(uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a)
        : r_{r}
        , g_{g}
        , b_{b}
        , a_{a}
    {
    }

    Color(uint8_t c)
        : r_{c}
        , g_{c}
        , b_{c}
        , a_{c}
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

private:
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
    uint8_t a_;
};

} // namespace glow
