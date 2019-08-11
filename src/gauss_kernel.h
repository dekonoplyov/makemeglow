#pragma once

#include <vector>
#include <cstddef>

namespace glow {

/*
 * <Description>  
 *    Returns right half of central row from Gaussian kernel
 *    https://en.wikipedia.org/wiki/Gaussian_blur
 *    This can be used to implement two pass Gaussian blur
 *    https://learnopengl.com/Advanced-Lighting/Bloom
 * <Input> 
 *    kernelSize :: size of Gaussian kernel
 *    sigma2 :: variance is Gaussian function
 * 
 */
std::vector<float> gauss1dKernel(size_t kernelSize, float sigma2);

} // namespace glow
