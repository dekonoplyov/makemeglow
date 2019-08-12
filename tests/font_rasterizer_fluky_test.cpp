#include "test_data.h"

#include "makemeglow/font_rasterizer.h"
#include "makemeglow/png.h"

#include <gtest/gtest.h>

#include <cmath>

bool compare(const glow::IntensityBuffer& expected, const glow::IntensityBuffer& actual)
{
    if (expected.width() != actual.width()) {
        EXPECT_TRUE(false)
            << "Buffer widths are different: "
            << expected.width() << " " << actual.width() << std::endl;
        return false;
    }

    if (expected.height() != actual.height()) {
        EXPECT_TRUE(false)
            << "Buffer heights are different: "
            << expected.height() << " " << actual.height() << std::endl;
        return false;
    }

    float sum = 0.f;
    float error = 0.f;
    for (size_t y = 0; y < expected.height(); ++y) {
        for (size_t x = 0; x < expected.width(); ++x) {
            sum += expected(x, y);
            const auto diff = static_cast<float>(expected(x, y))
                - static_cast<float>(actual(x, y));
            error += std::fabs(diff);
        }
    }

    return error / sum <= 0.03f;
}

/*
 * This is not the best way to test rendering
 * different buffer sizes, intensities could crash comparison
 * This is why they only warn and not crashing
 */
TEST(FontRasterizerTest, RasterASCII)
{
    using namespace glow;
    const auto fontnames = {"NotoSansJP-Black.otf", "Farro-Light.ttf"};
    const auto sizes = {10, 15, 20, 25, 30, 40, 50, 80, 100, 200};
    const auto allASCII = visibleASCIIChars();
    for (const auto& fontname : fontnames) {
        auto rasretizer = makeFontRasterizer(fontPath(fontname));
        for (const auto size : sizes) {
            const auto buffer = rasretizer->rasterize(allASCII, size);
            const auto canon = readGrayPng(grayPngPath(pngName(fontname, size)));
            EXPECT_TRUE(compare(buffer, canon))
                << "Png comparison has failed."
                << "Uncomment the next source line to write png and see diff";
            // writePng("./" + pngName(fontname, size), buffer);
        }
    }
}
