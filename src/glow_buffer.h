#pragma once

#include "makemeglow/buffer.h"
#include "makemeglow/color.h"

namespace glow {

ColorBuffer glowBuffer(
    const IntensityBuffer& buffer,
    const std::vector<float>& weights,
    Color textColor,
    Color backgroundColor);

} // namespace glow
