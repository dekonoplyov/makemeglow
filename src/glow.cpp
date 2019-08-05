#include "makemeglow/glow.h"
#include "font_rasterizer.h"

#include <algorithm>
#include <cstdint>
#include <exception>
#include <iostream>

namespace glow {

ColorBuffer glowText(IntensityBuffer buffer)
{
    const size_t margin = 10;
    ColorBuffer glowedBuffer{buffer.width() + 2 * margin, buffer.height() + 2 * margin};

    const float multiplier = 1.f / (margin * margin);
    const int from = -static_cast<int>(3 * margin / 2);
    const int to = static_cast<int>(margin) + from;

    std::cout << multiplier << ' ' << from << ' ' << to << std::endl;

    for (size_t x = 0; x < glowedBuffer.width(); ++x) {
        for (size_t y = 0; y < glowedBuffer.height(); ++y) {
            uint32_t intensityAcc = 0;

            for (int i = from; i < to; ++i) {
                for (int j = from; j < to; ++j) {
                    int p = i + x;
                    int q = j + y;
                    if (p < 0 || p >= buffer.width() || q < 0 || q >= buffer.height()) {
                        continue;
                    }

                    intensityAcc += buffer.at(p, q);
                }
            }

            uint8_t intensity = static_cast<uint8_t>(std::clamp(intensityAcc * multiplier, 0.f, 255.f));
            if (x > margin && x < (buffer.width() + margin)
                && y > margin && y < (buffer.height() + margin)) {
                    intensity = std::max(intensity, buffer.at(x - margin, y - margin));
            }

            Color c{255u, 0u, 255u, intensity};
            glowedBuffer.at(x, y) = c;
        }
    }

    return glowedBuffer;
}

void some(FontRasterizer& rr, size_t px)
{
    const std::string s = "./data/" + std::to_string(px) + "px.png";
    const std::string text{"FontRasterizer"};
    writePng(s, glowText(rr.rasterize(text, px)));
}

void foo()
{
    FontRasterizer rr{"/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf"};
    some(rr, 10);
    some(rr, 15);
    some(rr, 20);
    some(rr, 25);
    some(rr, 30);
    some(rr, 40);
    some(rr, 50);
    some(rr, 80);
    some(rr, 100);
}

} // namespace glow
