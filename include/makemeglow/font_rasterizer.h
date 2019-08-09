#pragma once

#include "makemeglow/buffer.h"

#include <memory>

namespace glow {

class FontRasterizer {
public:
    FontRasterizer(const std::string& font);
    ~FontRasterizer();

    IntensityBuffer rasterize(const std::string& text, size_t pixelSize, size_t margin = 0);

private:
    class FontRasterizerImpl;
    std::unique_ptr<FontRasterizerImpl> impl_;
};

} // namespace glow
