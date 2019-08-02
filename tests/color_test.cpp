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
