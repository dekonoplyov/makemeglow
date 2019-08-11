#include "test_data.h"

#include "makemeglow/png.h"

#include <gtest/gtest.h>

TEST(PngTest, Read)
{
    ASSERT_NO_THROW(glow::readGrayPng(glow::grayPngPath("Farro-Light.ttf10.png")));
}

TEST(PngTest, ReadFail)
{
    using namespace glow;
    ASSERT_THROW(readGrayPng(glow::grayPngPath("some.png")), std::runtime_error);
    ASSERT_THROW(readGrayPng(glow::rgbPngPath("Farro-Light.ttf10.png")), std::runtime_error);
}

TEST(PngTest, WriteFail)
{
    using namespace glow;
    ASSERT_THROW(writePng("/totally/not/existent/path.png", ColorBuffer{0, 0}), std::runtime_error);
    ASSERT_THROW(writePng("/totally/not/existent/path.png", IntensityBuffer{0, 0}), std::runtime_error);
}
