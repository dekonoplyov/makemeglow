#include "makemeglow/color.h"
#include "makemeglow/glow.h"
#include "makemeglow/png.h"

int main()
{
    const std::string font = "../../tests/data/font/Farro-Light.ttf";
    const std::string text = "Hey you, out there in the cold. Getting lonely, getting old. Can you feel me?";
    const size_t size = 30;
    const glow::Color textColor{250, 100, 255};
    const glow::Color backgroundColor{0u};
    const glow::GlowParams params{11, 5.f};
    const auto buffer = glow::makeRasterizer(font)
                            ->rasterize(text, size,
                                textColor, backgroundColor,
                                glow::GlowParams{params.kernelSize, params.sigma});
    glow::writePng("./text.png", buffer);
}
