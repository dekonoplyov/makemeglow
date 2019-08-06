#include "gauss.h"

#include <gtest/gtest.h>

namespace {

void checkEquality(const std::vector<float>& expected, const std::vector<float> actual)
{
    ASSERT_EQ(actual.size(), expected.size());
    for (size_t idx = 0; idx < expected.size(); ++idx) {
        ASSERT_FLOAT_EQ(actual[idx], expected[idx]);
    }
}

} // namespace

TEST(GaussTest, ZeroRadius)
{
    const auto weights = glow::createGauss1dKernel(0);
    ASSERT_TRUE(weights.empty());
}

TEST(GaussTest, EvenRadius)
{
    const auto weights = glow::createGauss1dKernel(6);
    ASSERT_EQ(weights.size(), 7);
}

TEST(GaussTest, Defaults)
{
    const std::vector<float> expected{0.39894345f, 0.24197143f, 0.053991124f, 0.0044318615f, 0.00013383062f};
    const auto weights = glow::createGauss1dKernel();
    checkEquality(expected, weights);
}

TEST(GaussTest, NonDefaults)
{
    const std::vector<float> expected{0.21260943, 0.20606813, 0.18762717};
    const auto weights = glow::createGauss1dKernel(3, 4.f);
    checkEquality(expected, weights);
}
