#pragma once

#include "test_data_path.h"

#include "makemeglow/buffer.h"

#include <string>

namespace glow {

inline std::string fontPath(const std::string& fontname)
{
    return std::string{TEST_DATA_PATH} + "/font/" + fontname;
}

inline std::string grayPngPath(const std::string& filename)
{
    return std::string{TEST_DATA_PATH} + "/png/gray/" + filename;
}

inline std::string rgbPngPath(const std::string& filename)
{
    return std::string{TEST_DATA_PATH} + "/png/rgb/" + filename;
}

inline std::string pngName(const std::string& fontname, size_t size)
{
    return fontname + std::to_string(size) + ".png";
}

inline std::string visibleASCIIChars()
{
    return " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
}

template <typename T>
bool isEquals(const Buffer<T>& l, const Buffer<T>& r)
{
    if (l.width() != r.width() || l.height() != r.height()) {
        return false;
    }

    // TODO consider other ways to compare png with threshold
    size_t mismatches = 0;
    for (size_t y = 0; y < l.height(); ++y) {
        for (size_t x = 0; x < l.width(); ++x) {
            if (l.at(x, y) != r.at(x, y)) {
                mismatches++;
            }
        }
    }

    return mismatches * 20 < l.data().size();
}

} // namespace glow
