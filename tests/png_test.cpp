#include "test_data.h"

#include "makemeglow/png.h"

#include <gtest/gtest.h>

TEST(PngTest, Read)
{
    using namespace glow;
    ASSERT_NO_THROW(readGrayPng(grayPngPath("Farro-Light.ttf10.png")));
    ASSERT_NO_THROW(readRGBPng(rgbPngPath("some.png")));
}

TEST(PngTest, ReadFail)
{
    using namespace glow;
    ASSERT_THROW(readRGBPng(grayPngPath("some.png")), std::runtime_error);
    ASSERT_THROW(readGrayPng(rgbPngPath("Farro-Light.ttf10.png")), std::runtime_error);
}

TEST(PngTest, WriteFail)
{
    using namespace glow;
    ASSERT_THROW(writePng("/totally/not/existent/path.png", ColorBuffer{0, 0}), std::runtime_error);
    ASSERT_THROW(writePng("/totally/not/existent/path.png", IntensityBuffer{0, 0}), std::runtime_error);
}
