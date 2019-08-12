#include "test_data.h"

#include "makemeglow/font_rasterizer.h"
#include "makemeglow/png.h"

#include <gtest/gtest.h>

#include <cmath>

// TODO make warnings when sizes are not equal
// and easy dump pictures to see if everything is ok
bool compare(const glow::IntensityBuffer& expected, const glow::IntensityBuffer& actual)
{
    EXPECT_TRUE(expected.width() == actual.width())
        << "Buffer widths are different: "
        << expected.width() << " " << actual.width() << std::endl;

    EXPECT_TRUE(expected.height() == actual.height())
        << "Buffer heights are different: "
        << expected.height() << " " << actual.height() << std::endl;

    const auto xOffset = static_cast<int>(expected.width()) - static_cast<int>(actual.width());
    const auto yOffset = static_cast<int>(expected.height()) - static_cast<int>(actual.height());

    float sum = 0.f;
    float error = 0.f;
    for (int y = 0; y < static_cast<int>(expected.height()); ++y) {
        for (int x = 0; x < static_cast<int>(expected.width()); ++x) {
            sum += expected(x, y);
            const auto diff = static_cast<float>(expected(x, y))
                - static_cast<float>(actual(x + xOffset, y + yOffset));
            error += std::fabs(diff);
        }
    }

    return error / sum <= 0.03f;
}

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
            ASSERT_TRUE(compare(buffer, canon));
        }
    }
}
