#pragma once

#include "makemeglow/color_buffer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace glow {

// TODO make nice api
class FontRasterizer {
public:
    FontRasterizer(const std::string& font);

    ~FontRasterizer();

    IntensityBuffer rasterize(const std::string& text, size_t pixelSize);

private:
    FT_Library library_;
    FT_Face face_;
};

} // namespace glow
