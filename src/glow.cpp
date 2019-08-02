#include "makemeglow/glow.h"
#include "font_rasterizer.h"

#include <cstdint>
#include <exception>
#include <iostream>

namespace glow {

void foo()
{
    writePng("./some.png", FontRasterizer{}.rasterize());
}

} // namespace glow
