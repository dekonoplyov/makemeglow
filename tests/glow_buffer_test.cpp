#include "glow_buffer.h"

#include "test_data.h"

#include <gtest/gtest.h>

namespace {

void fillBuffer(glow::IntensityBuffer& buffer)
{
    for (size_t y = 0; y < buffer.height(); ++y) {
        for (size_t x = 0; x < buffer.width(); ++x) {
            buffer(x, y) = static_cast<uint8_t>(50 * x * y);
        }
    }
}

glow::ColorBuffer getGlowed()
{
    glow::ColorBuffer glowed{3, 4};
    glowed.data() = {
        {0, 0, 0}, {2, 0, 2}, {3, 0, 3},
        {2, 0, 2}, {48, 3, 39}, {97, 7, 78},
        {6, 0, 5}, {97, 7, 78}, {195, 15, 156},
        {5, 0, 4}, {146, 11, 117}, {42, 3, 34}};
    return glowed;
}

} // namespace

TEST(GlowBuffer, EmptyWeights)
{
    using namespace glow;
    IntensityBuffer buffer{10, 10};
    Color text{10u};
    Color background{120u};
    ASSERT_THROW(glowBuffer(buffer, /*weights*/ {}, text, background), std::runtime_error);
}

TEST(GlowBuffer, Glow)
{
    using namespace glow;
    IntensityBuffer buffer{3, 4};
    fillBuffer(buffer);
    Color text{250, 20, 200};
    Color background{120u};
    ASSERT_TRUE(isEquals(getGlowed(), glowBuffer(buffer, /*weights*/ {0.2f, 0.15f}, text, background)));
}
