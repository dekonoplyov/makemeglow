#pragma once

#include <vector>

namespace glow {

// TODO fix description
/*
 * returns right half of Gaussian curve
 * we can skip other half due to its symmetric shape
 * if radius is even number greater than zero it will be increased to the nearest odd
 */
std::vector<float> createGauss1dKernel(size_t radius = 5, float sigma = 1.f);

} // namespace glow
