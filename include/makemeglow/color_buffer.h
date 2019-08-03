#pragma once

#include "color.h"

#include <cstddef>
#include <exception>
#include <string>
#include <vector>

namespace glow {

/**
 * Coordinates goes in these directions 
 * (0, 0).−−> x
 *       |...
 *       |..(3, 2)
 *       V
 *       y
 */
class ColorBuffer {
public:
    using Buffer = std::vector<Color>;

    ColorBuffer(size_t width, size_t height)
        : width_{width}
        , height_{height}
        , buffer_{Buffer(width_ * height_)}
    {
    }

    ColorBuffer(size_t width, size_t height, Buffer buffer)
        : width_{width}
        , height_{height}
        , buffer_{std::move(buffer)}
    {
    }

    const std::vector<Color>& buffer() const
    {
        return buffer_;
    }

    std::vector<Color>& buffer()
    {
        return buffer_;
    }

    size_t width() const
    {
        return width_;
    }

    size_t height() const
    {
        return height_;
    }

    Color at(size_t x, size_t y) const
    {
        return buffer_.at(x + y * width_);
    }

    Color& at(size_t x, size_t y)
    {
        return buffer_.at(x + y * width_);
    }

    void set(size_t x, size_t y, Color c)
    {
        buffer_.at(x + y * width_) = c;
    }

private:
    size_t width_;
    size_t height_;
    std::vector<Color> buffer_;
};

void writePng(const std::string& filename, const ColorBuffer& buffer);

} // namespace glow
