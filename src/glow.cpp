#include "makemeglow/glow.h"
#include "font_rasterizer.h"

#include <cstdint>
#include <exception>
#include <iostream>

namespace glow {

ColorBuffer glowText(ColorBuffer buffer)
{
    ColorBuffer glowedBuffer{buffer.width(), buffer.height()};
    for (size_t y = 0; y < buffer.height(); ++y) {
        for (size_t x = 0; x < buffer.width(); ++x) {
            Color res = buffer.at(x, y);
            for (int i = x - 3; i < x + 4; ++i) {
                if (i < 0 || i >= buffer.width()) {
                    continue;
                }
                res = res + buffer.at(i, y);
                res = res * 0.14f;
                res = Color{res.r(), res.g(), res.a()};
            }

            glowedBuffer.set(x, y, res);
        }
    }

    for (size_t y = 0; y < buffer.height(); ++y) {
        for (size_t x = 0; x < buffer.width(); ++x) {
             glowedBuffer.set(x, y, buffer.at(x, y) + glowedBuffer.at(x, y));
        }
    }

    for (size_t y = 0; y < buffer.height(); ++y) {
        for (size_t x = 0; x < buffer.width(); ++x) {
            Color res = buffer.at(x, y);
            for (int i = y - 3; i < y + 4; ++i) {
                if (i < 0 || i >= buffer.height()) {
                    continue;
                }
                res = res + buffer.at(x, i);
                res = res * 0.14f;
            }

            glowedBuffer.set(x, y, res);
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
    FontRasterizer rr{};
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
