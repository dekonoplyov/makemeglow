#include "makemeglow/glow.h"
#include "font_rasterizer.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
#include <iostream>

namespace glow {

namespace {

ColorBuffer gaussianBlur(const IntensityBuffer& buffer, const std::vector<float>& weights)
{
    const int yLimit = static_cast<int>(buffer.height());
    const int xLimit = static_cast<int>(buffer.width());
    const int radius = static_cast<int>(weights.size());

    IntensityBuffer horizontalPass{buffer.width(), buffer.height()};
    for (int y = 0; y < yLimit; ++y) {
        for (int x = 0; x < xLimit; ++x) {
            float res = static_cast<float>(buffer.at(x, y)) * weights[0];
            for (int i = 1; i < radius; ++i) {
                if (x + i < xLimit) {
                    res += static_cast<float>(buffer.at(x + i, y)) * weights[i];
                }
                if (x - i >= 0) {
                    res += static_cast<float>(buffer.at(x - i, y)) * weights[i];
                }
            }
            horizontalPass.at(x, y) = static_cast<uint8_t>(std::clamp(res, 0.f, 255.f));
        }
    }

    ColorBuffer verticalPass{buffer.width(), buffer.height()};
    for (int y = 0; y < yLimit; ++y) {
        for (int x = 0; x < xLimit; ++x) {
            float res = static_cast<float>(horizontalPass.at(x, y)) * weights[0];
            for (int i = 1; i < radius; ++i) {
                if (y + i < yLimit) {
                    res += static_cast<float>(horizontalPass.at(x, y + i)) * weights[i];
                }
                if (y - i >= 0) {
                    res += static_cast<float>(horizontalPass.at(x, y - i)) * weights[i];
                }
            }

            const auto intensity = std::max(static_cast<uint8_t>(std::clamp(res, 0.f, 255.f)), buffer.at(x, y));
            const Color c{
                static_cast<uint8_t>(231 * (intensity / 255.f)),
                static_cast<uint8_t>(89 * (intensity / 255.f)),
                static_cast<uint8_t>(163 * (intensity / 255.f)),
                255};
            verticalPass.at(x, y) = c;
        }
    }

    return verticalPass;
}

} // namespace

void some(FontRasterizer& rr, size_t px)
{
    const std::string s = "./junk/" + std::to_string(px) + "px.png";
    const std::string text{"makemeglow"};
    const std::vector<float> weights{0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f};
    writePng(s, gaussianBlur(rr.rasterize(text, px), weights));
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
    some(rr, 150);
    some(rr, 200);
}

} // namespace glow
