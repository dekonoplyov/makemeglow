#include "test_data.h"

#include "makemeglow/font_rasterizer.h"
#include "makemeglow/png.h"

#include <gtest/gtest.h>

#include <cmath>

// TODO consider other thresholds
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

TEST(FontRasterizerTest, NonExistentFont)
{
    using namespace glow;
    ASSERT_THROW(makeFontRasterizer("/non/existent/font.ttf"), std::runtime_error);
}

TEST(FontRasterizerTest, CorruptedFont)
{
    using namespace glow;
    ASSERT_THROW(makeFontRasterizer(fontPath("my_font.txt")),
        std::runtime_error);
}

TEST(FontRasterizerTest, NonScalableFont)
{
    using namespace glow;
    ASSERT_THROW(makeFontRasterizer(fontPath("cherry-10-r.bdf")),
        std::runtime_error);
}

TEST(FontRasterizerTest, ExistentFont)
{
    using namespace glow;
    ASSERT_NO_THROW(makeFontRasterizer(fontPath("Farro-Light.ttf")));
    ASSERT_NO_THROW(makeFontRasterizer(fontPath("Hado2-Regular.otf")));
    ASSERT_NO_THROW(makeFontRasterizer(fontPath("NotoSansJP-Black.otf")));
}

TEST(FontRasterizerTest, CheckMargins)
{
    using namespace glow;
    auto rasretizer = makeFontRasterizer(fontPath("Farro-Light.ttf"));
    for (const auto margin : {0, 10}) {
        auto intensities = rasretizer->rasterize("", /*pxSize*/ 42, margin);
        ASSERT_EQ(intensities.width(), 2 * margin);
        ASSERT_EQ(intensities.height(), 2 * margin);
    }
}

TEST(FontRasterizerTest, NonASCII)
{
    using namespace glow;
    auto rasretizer = makeFontRasterizer(fontPath("Farro-Light.ttf"));
    ASSERT_NO_THROW(rasretizer->rasterize("привет", 10));
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
