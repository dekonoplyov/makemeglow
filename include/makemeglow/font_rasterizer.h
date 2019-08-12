#pragma once

#include "makemeglow/buffer.h"

#include <memory>

namespace glow {

/*
 * Use this class if you need to rasterize text without color
 */
class FontRasterizerInterface {
public:
    virtual ~FontRasterizerInterface(){};
    virtual IntensityBuffer rasterize(const std::string& text, size_t pixelSize, size_t margin = 0) = 0;
};

using FontRasterizerPtr = std::unique_ptr<FontRasterizerInterface>;

FontRasterizerPtr makeFontRasterizer(const std::string& font);

} // namespace glow
