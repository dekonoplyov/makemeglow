#include "makemeglow/glow.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <cstdint>

namespace glow {

int foo()
{
    const size_t HEIGHT = 100;
    const size_t WIDTH = 100;
    uint8_t ramp[WIDTH * HEIGHT * 3];
    stbi_write_png("./some.png", WIDTH, HEIGHT, 3, &ramp, 3 * WIDTH);
    return 42;
}

} // namespace glow
