#include "gauss_kernel.h"

#include "makemeglow/buffer.h"

#include <cmath>
#include <stdexcept>

namespace glow {

namespace {

inline float distance(int x, int y, int offset)
{
    return static_cast<float>((x - offset) * (x - offset) + (y - offset) * (y - offset));
}

} // namespace

std::vector<float> gauss1dKernel(size_t kernelSize, float sigma2)
{
    if (kernelSize % 2 == 0) {
        throw std::runtime_error{"Use odd kernel sizes"};
    }

    if (kernelSize == 1) {
        return {1.f};
    }

    const size_t offset = kernelSize / 2;
    Buffer<float> weights{kernelSize, kernelSize};
    float sum = 0.0f;

    for (size_t j = 0; j < kernelSize; ++j) {
        for (size_t i = 0; i < kernelSize; ++i) {
            weights(i, j) = std::exp(-distance(i, j, offset) / (2.f * sigma2));
            sum += weights(i, j);
        }
    }

    sum /= sqrt(2.f * M_PI * sigma2);

    for (size_t j = 0; j < weights.width(); ++j) {
        for (size_t i = 0; i < weights.height(); ++i) {
            weights(i, j) /= sum;
        }
    }

    // take right half of central row
    std::vector<float> kernel;
    for (size_t i = 0; i <= offset; ++i) {
        kernel.push_back(weights(i + offset, offset));
    }

    return kernel;
}

} // namespace glow
