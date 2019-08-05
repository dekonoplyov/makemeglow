#include "makemeglow/glow.h"
#include "font_rasterizer.h"

#include <cstdint>
#include <exception>
#include <iostream>

namespace glow {

ColorBuffer glowText(ColorBuffer buffer, size_t margin)
{
    ColorBuffer glowedBuffer{buffer.width(), buffer.height()};

    const float multiplier = 1.f / margin;
    const int from = -static_cast<int>(margin / 2);
    const int to = static_cast<int>(margin) + from;

    std::cout << multiplier << ' ' << from << ' ' << to << std::endl;

    for (size_t x = 0; x < buffer.width(); ++x) {
        for (size_t y = 0; y < buffer.height(); ++y) {
            float res = 0.f;
            for (int i = from; i < to; ++i) {
                for (int j = from; j < to; ++j) {
                    int p = i + x;
                    int q = j + y;
                    if (p < 0 || p >= buffer.width() || q < 0 || q >= buffer.height()) {
                        continue;
                    }

                    res += buffer.at(p, q).a() * multiplier;
                }
            }
            
            Color at = buffer.at(x, y);
            Color c{255, 255, 255, static_cast<uint8_t>(res)};
            glowedBuffer.set(x, y, c);
        }
    }

    for (size_t y = 0; y < buffer.height(); ++y) {
        for (size_t x = 0; x < buffer.width(); ++x) {
            glowedBuffer.set(x, y, buffer.at(x, y) + glowedBuffer.at(x, y));
        }
    }

    return glowedBuffer;
}

void foo()
{
    FontRasterizer rr{"/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf"};
    writePng("./data/10px.png", rr.rasterize(10));
    writePng("./data/15px.png", rr.rasterize(15));
    writePng("./data/20px.png", rr.rasterize(20));
    writePng("./data/25px.png", rr.rasterize(25));
    writePng("./data/30px.png", rr.rasterize(30));
    writePng("./data/40px.png", rr.rasterize(40));
    writePng("./data/50px.png", rr.rasterize(50));
    writePng("./data/80px.png", rr.rasterize(80));
    writePng("./data/100px.png", rr.rasterize(100));
}

} // namespace glow
