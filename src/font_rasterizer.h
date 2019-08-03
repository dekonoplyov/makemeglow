#pragma once

#include "makemeglow/color_buffer.h"
#include "makemeglow/color_buffer.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace glow {

class FontRasterizer {
public:
    FontRasterizer();

    ~FontRasterizer();

    ColorBuffer rasterize();

private:
    ColorBuffer createPropperSizeBuffer(const std::string& text);

private:
    FT_Library library_;
    FT_Face face_;
};

} // namespace glow
