#include "makemeglow/color.h"

#include <gtest/gtest.h>

TEST(ColorTest, OneChanelConstructor)
{
    glow::Color c{uint8_t{10}};
    ASSERT_EQ(c.r(), 10);
    ASSERT_EQ(c.g(), 10);
    ASSERT_EQ(c.b(), 10);
    ASSERT_EQ(c.a(), 255);
}

TEST(ColorTest, FourChanelsConstructorInInt)
{
    {
        glow::Color c{0x12345678u};
        ASSERT_EQ(c.r(), 18);
        ASSERT_EQ(c.g(), 52);
        ASSERT_EQ(c.b(), 86);
        ASSERT_EQ(c.a(), 120);
    }

    {
        glow::Color c{123454u};
        ASSERT_EQ(c.r(), 0);
        ASSERT_EQ(c.g(), 1);
        ASSERT_EQ(c.b(), 226);
        ASSERT_EQ(c.a(), 62);
    }
}

TEST(ColorTest, FourChanelsConstructor)
{
    {
        glow::Color c{10, 11, 12, 13};
        ASSERT_EQ(c.r(), 10);
        ASSERT_EQ(c.g(), 11);
        ASSERT_EQ(c.b(), 12);
        ASSERT_EQ(c.a(), 13);
    }

    {
        glow::Color c{10, 11, 12};
        ASSERT_EQ(c.r(), 10);
        ASSERT_EQ(c.g(), 11);
        ASSERT_EQ(c.b(), 12);
        ASSERT_EQ(c.a(), 255);
    }
}

TEST(ColorTest, Blend)
{
    {
        const glow::Color bg{200, 117, 12, 250};
        const glow::Color fg{42, 13, 150, 130};
        auto blended = glow::blendColors(bg, fg);

        ASSERT_EQ(blended.r(), 118);
        ASSERT_EQ(blended.g(), 63);
        ASSERT_EQ(blended.b(), 82);
        ASSERT_EQ(blended.a(), 255);
    }

    {
        const glow::Color bg{200, 0, 12};
        const glow::Color fg{0, 13, 150};
        auto blended = glow::blendColors(bg, fg);

        ASSERT_EQ(blended.r(), 0);
        ASSERT_EQ(blended.g(), 12);
        ASSERT_EQ(blended.b(), 149);
        ASSERT_EQ(blended.a(), 255);
    }
}

TEST(ColorTest, ParseFail)
{
    ASSERT_THROW(glow::parseColor(""), std::runtime_error);
    ASSERT_THROW(glow::parseColor("#000"), std::runtime_error);
    ASSERT_THROW(glow::parseColor("#zcb000"), std::runtime_error);
    // strtoul has no way to handle base 16 format
    ASSERT_NO_THROW(glow::parseColor("zcb000"));
}

TEST(ColorTest, Parse)
{
    const auto color = glow::parseColor("aabbcc");
    ASSERT_EQ(color.r(), 170);
    ASSERT_EQ(color.g(), 187);
    ASSERT_EQ(color.b(), 204);
    ASSERT_EQ(color.a(), 255);
}
