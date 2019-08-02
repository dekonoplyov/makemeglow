#include "color.h"

#include <cstddef>
#include <string>
#include <vector>

namespace glow {

class Png {
public:
    Png(size_t width, size_t height)
        : width_{width}
        , height_{height}
        , buffer_{std::vector<Color>(width_ * height_)}
    {
    }

    void write(const std::string& filename) const;
    

    const std::vector<Color>& buffer() const
    {
        return buffer_;
    }

    std::vector<Color>& buffer()
    {
        return buffer_;
    }

private:
    size_t width_;
    size_t height_;
    std::vector<Color> buffer_;
};

} // namespace glow
