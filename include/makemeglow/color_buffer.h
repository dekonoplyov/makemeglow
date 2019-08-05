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
template <typename T>
class Buffer {
public:
    Buffer(size_t width, size_t height)
        : width_{width}
        , height_{height}
        , buffer_{std::vector<T>(width_ * height_)}
    {
    }

    const std::vector<T>& data() const
    {
        return buffer_;
    }

    std::vector<T>& data()
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

    T at(size_t x, size_t y) const
    {
        return buffer_.at(x + y * width_);
    }

    T& at(size_t x, size_t y)
    {
        return buffer_.at(x + y * width_);
    }

private:
    size_t width_;
    size_t height_;
    std::vector<T> buffer_;
};

using IntensityBuffer = Buffer<uint8_t>;
using ColorBuffer = Buffer<Color>;

void writePng(const std::string& filename, const ColorBuffer& buffer);

} // namespace glow
