#include "test_data.h"

#include "makemeglow/font_rasterizer.h"
#include "makemeglow/png.h"

#include <gtest/gtest.h>

std::string fontPath(const std::string& fontname, bool scalable = true)
{
    return std::string{TEST_DATA_PATH} + "/font/"
        + (scalable ? "scalable" : "non_scalable")
        + "/" + fontname;
}

TEST(FontRasterizerTest, NonExistentFont)
{
    ASSERT_THROW(glow::FontRasterizer{"/non/existent/font.ttf"}, std::runtime_error);
}

TEST(FontRasterizerTest, CorruptedFont)
{
    ASSERT_THROW(glow::FontRasterizer{fontPath("corrupted-cherry-10-r.bdf", false)},
        std::runtime_error);
}

TEST(FontRasterizerTest, ExistentFont)
{
    ASSERT_NO_THROW(glow::FontRasterizer{fontPath("Farro-Light.ttf")});
    ASSERT_NO_THROW(glow::FontRasterizer{fontPath("Hado2-Regular.otf")});
    ASSERT_NO_THROW(glow::FontRasterizer{fontPath("NotoSansJP-Black.otf")});
    ASSERT_NO_THROW(glow::FontRasterizer{fontPath("cherry-10-r.bdf", false)});
}

TEST(FontRasterizerTest, ScaleNonScalableFont)
{
    auto rr = glow::FontRasterizer{fontPath("cherry-10-r.bdf", false)};

    ASSERT_THROW(rr.rasterize("", 11), std::runtime_error);
    ASSERT_NO_THROW(rr.rasterize("", 10));
}

TEST(FontRasterizerTest, CheckMargins)
{
    auto rr = glow::FontRasterizer{fontPath("Farro-Light.ttf")};

    for (const auto margin : {0, 10}) {
        auto intensities = rr.rasterize("", /*pxSize*/ 42, margin);
        ASSERT_EQ(intensities.width(), 2 * margin);
        ASSERT_EQ(intensities.height(), 2 * margin);
    }
}

TEST(FontRasterizerTest, NonASCII)
{
    auto rr = glow::FontRasterizer{fontPath("Farro-Light.ttf")};
    ASSERT_NO_THROW(rr.rasterize("привет", 10));
}

// TODO one color png write
