#include "makemeglow/png.h"

namespace glow {

namespace {

class PngWriter {
    PngWriter(const std::string& filename)
        : filename_{filename}
    {
    }

private:
    std::string filename_;
};

} // namespace

void Png::write(const std::string& filename) const
{
}

} // namespace glow
