#include "test_data.h"

#include "makemeglow/font_rasterizer.h"
#include "makemeglow/png.h"

#include <gtest/gtest.h>

TEST(FontRasterizerTest, NonExistentFont)
{
    using namespace glow;
    ASSERT_THROW(FontRasterizer{"/non/existent/font.ttf"}, std::runtime_error);
}

TEST(FontRasterizerTest, CorruptedFont)
{
    using namespace glow;
    ASSERT_THROW(FontRasterizer{fontPath("my_font.txt")},
        std::runtime_error);
}

TEST(FontRasterizerTest, NonScalableFont)
{
    using namespace glow;
    ASSERT_THROW(FontRasterizer{fontPath("cherry-10-r.bdf")},
        std::runtime_error);
}

TEST(FontRasterizerTest, ExistentFont)
{
    using namespace glow;
    ASSERT_NO_THROW(FontRasterizer{fontPath("Farro-Light.ttf")});
    ASSERT_NO_THROW(FontRasterizer{fontPath("Hado2-Regular.otf")});
    ASSERT_NO_THROW(FontRasterizer{fontPath("NotoSansJP-Black.otf")});
}

TEST(FontRasterizerTest, CheckMargins)
{
    using namespace glow;
    FontRasterizer rasretizer{fontPath("Farro-Light.ttf")};
    for (const auto margin : {0, 10}) {
        auto intensities = rasretizer.rasterize("", /*pxSize*/ 42, margin);
        ASSERT_EQ(intensities.width(), 2 * margin);
        ASSERT_EQ(intensities.height(), 2 * margin);
    }
}

TEST(FontRasterizerTest, NonASCII)
{
    using namespace glow;
    FontRasterizer rr{fontPath("Farro-Light.ttf")};
    ASSERT_NO_THROW(rr.rasterize("привет", 10));
}

TEST(FontRasterizerTest, RasterASCII)
{
    using namespace glow;
    const auto fontnames = {"NotoSansJP-Black.otf", "Farro-Light.ttf"};
    const auto sizes = {10, 15, 20, 25, 30, 40, 50, 80, 100, 200};
    const auto allASCII = visibleASCIIChars();
    for (const auto& fontname : fontnames) {
        FontRasterizer rasretizer{fontPath(fontname)};
        for (const auto size : sizes) {
            const auto buffer = rasretizer.rasterize(allASCII, size);
            const auto canon = readGrayPng(grayPngPath(pngName(fontname, size)));
            ASSERT_TRUE(isEquals(buffer, canon));
        }
    }
}
