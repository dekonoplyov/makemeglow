#include "gauss.h"

#include <math.h>

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

    float s = 2.0 * sigma * sigma;
    for (int i = 0; i < radius; ++i) {
        // float r = 
        // weights[i] = (std::exp(-(r * r) / s)) / (M_PI * s);
    }

    // TODO make real compute

    return {0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f};
}

} // namespace glow
