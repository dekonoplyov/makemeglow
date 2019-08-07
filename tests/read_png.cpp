#include "read_png.h"

#include <png.h>

#include <functional>
#include <stdexcept>

namespace glow {

namespace {

class LibPngReadWrapper {
public:
    LibPngReadWrapper(const std::string& filename)
        : file{nullptr}
        , png{nullptr}
        , info{nullptr}
        , row{nullptr}
    {
        file = fopen(filename.c_str(), "rb");
        if (file == nullptr) {
            throw std::runtime_error{"Could not open file for reading"};
        }

        png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png == nullptr) {
            throw std::runtime_error{"Could not allocate read struct"};
        }

        info = png_create_info_struct(png);
        if (info == nullptr) {
            throw std::runtime_error{"Could not allocate info struct"};
        }

        // libPNG exception handling
        if (setjmp(png_jmpbuf(png))) {
            throw std::runtime_error{"Error during png reading"};
        }
    }

    ~LibPngReadWrapper()
    {
        if (file != nullptr) {
            fclose(file);
        }
        if (info != nullptr) {
            png_destroy_info_struct(png, &info);
        }
        if (png != nullptr) {
            png_destroy_read_struct(&png, nullptr, nullptr);
        }
        if (row != nullptr) {
            free(row);
        }
    }

public:
    FILE* file;
    png_structp png;
    png_infop info;
    png_bytep row;
};

} // namespace

IntensityBuffer readPng(const std::string& filename)
{
    LibPngReadWrapper reader{filename};

    png_init_io(reader.png, reader.file);
    png_read_info(reader.png, reader.info);

    if (png_get_bit_depth(reader.png, reader.info) != 8) {
        throw std::runtime_error{"some shit"};
    }

    if (png_get_color_type(reader.png, reader.info) != PNG_COLOR_TYPE_GRAY) {
        throw std::runtime_error{"other shit"};
    }

    const auto width = png_get_image_width(reader.png, reader.info);
    const auto height = png_get_image_height(reader.png, reader.info);
    IntensityBuffer buffer{width, height};
    reader.row = (png_bytep)malloc(sizeof(png_byte) * width);
    for (size_t y = 0; y < height; y++) {
        png_read_row(reader.png, reader.row, nullptr);
        for (size_t x = 0; x < width; ++x) {
            buffer.at(x, y) = reader.row[x];
        }
    }

    return buffer;
}

} // namespace glow
