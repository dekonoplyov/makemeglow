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

TEST(GaussTest, EvenKernelSize)
{
    ASSERT_THROW(glow::gauss1dKernel(0, 1.f), std::runtime_error);
    ASSERT_THROW(glow::gauss1dKernel(6, 16.f), std::runtime_error);
}

TEST(GaussTest, Values)
{
    {
        const std::vector<float> expected{1.f};
        const auto weights = glow::gauss1dKernel(1, 4.f);
        checkEquality(expected, weights);
    }

    {
        const std::vector<float> expected{0.18092f, 0.168448f, 0.13595733f, 0.0951256f, 0.0576966f};
        const auto weights = glow::gauss1dKernel(9, 7.f);
        checkEquality(expected, weights);
    }
}
