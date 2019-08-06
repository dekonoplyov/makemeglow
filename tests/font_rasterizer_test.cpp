#include "font_rasterizer.h"

#include <gtest/gtest.h>

TEST(FontRasterizerTest, NonExistentFont)
{
    ASSERT_THROW(glow::FontRasterizer{"/non/existent/font.ttf"}, std::runtime_error);
}

// TODO
// throw non scalable font
// non ascii letters
//  
