#include "gauss.h"

#include <cmath>

namespace glow {

std::vector<float> createGauss1dKernel(size_t radius, float sigma)
{
    if (radius == 0) {
        return {};
    }

    if (radius % 2 == 0) {
        radius += 1;
    }

    std::vector<float> weights(radius);

    const float sigma2 = 2.f * sigma * sigma;
    float sum = 0.f;
    for (size_t i = 0; i < radius; ++i) {
        weights[i] = std::exp(-static_cast<float>(i * i) / sigma2);
        sum += 2.f * weights[i];
    }

    sum -= weights[0];

    for (size_t i = 0; i < radius; ++i) {
        weights[i] /= sum;
    }

    return weights;
}

} // namespace glow
