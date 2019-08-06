#include "makemeglow/glow.h"

#include <iostream>

int main()
{
    const auto sizes = {10, 15, 20, 25, 30, 40, 50, 80, 100, 150, 200};
    const std::string text = "Some meaningfull text";
    const std::string font = "/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf";
    const glow::Color textColor{231, 17, 46};
    const glow::Color backgroundColor{};

    for (const auto size : sizes) {
        const std::string outputPath = "./junk/" + std::to_string(size) + "px.png";
        const auto buffer = glow::rasterize(text, font, size, textColor, backgroundColor);
        glow::writePng(outputPath, buffer);
    }

    return 0;
}
