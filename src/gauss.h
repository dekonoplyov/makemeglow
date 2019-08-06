#pragma once

#include <vector>

namespace glow {

std::vector<float> createGauss1dKernel(size_t radius = 5, float sigma = 1.f);

} // namespace glow
