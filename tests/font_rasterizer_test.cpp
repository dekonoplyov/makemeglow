#include "test_data.h"

#include "makemeglow/font_rasterizer.h"
#include "makemeglow/png.h"

#include <gtest/gtest.h>

#include <cmath>

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
