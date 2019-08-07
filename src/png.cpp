#include "makemeglow/png.h"

#include <png.h>

#include <functional>
#include <stdexcept>

namespace glow {

namespace {

class LibPngWrapper {
public:
    LibPngWrapper(const std::string& filename)
        : filename{filename}
        , file_ptr{nullptr}
        , png_ptr{nullptr}
        , info_ptr{nullptr}
        , row{nullptr}
    {
        file_ptr = fopen(filename.c_str(), "wb");
        if (file_ptr == nullptr) {
            throw std::runtime_error{"Could not open file for writing"};
        }

        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (png_ptr == nullptr) {
            throw std::runtime_error{"Could not allocate write struct"};
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == nullptr) {
            throw std::runtime_error{"Could not allocate info struct"};
        }

        // libPNG exception handling
        if (setjmp(png_jmpbuf(png_ptr))) {
            throw std::runtime_error{"Error during png creation"};
        }
    }

    ~LibPngWrapper()
    {
        if (file_ptr != nullptr) {
            fclose(file_ptr);
        }
        if (info_ptr != nullptr) {
            png_destroy_info_struct(png_ptr, &info_ptr);
        }
        if (png_ptr != nullptr) {
            png_destroy_write_struct(&png_ptr, nullptr);
        }
        if (row != nullptr) {
            free(row);
        }
    }

public:
    const std::string& filename;
    FILE* file_ptr;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep row;
};

void write(const std::string& filename,
    int width, int height,
    int colorMode, int stride,
    std::function<void(png_structp png_ptr, png_bytep)> writeCallback)
{
    LibPngWrapper png{filename};
    png_init_io(png.png_ptr, png.file_ptr);

    // Write header (8 bit colour depth)
    png_set_IHDR(png.png_ptr, png.info_ptr,
        width, height,
        /*bit depth*/ 8,
        colorMode, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png.png_ptr, png.info_ptr);

    png.row = (png_bytep)malloc(stride * width * sizeof(png_byte));
    writeCallback(png.png_ptr, png.row);

    png_write_end(png.png_ptr, nullptr);
}

} // namespace

void writePng(const std::string& filename, const ColorBuffer& buffer)
{
    write(filename,
        buffer.width(), buffer.height(),
        PNG_COLOR_TYPE_RGBA, /*stride*/ 4,
        [&buffer](png_structp png_ptr, png_bytep row) {
            for (size_t y = 0; y < buffer.height(); y++) {
                for (size_t x = 0; x < buffer.width(); x++) {
                    png_byte* currentPixel = &row[x * 4];
                    const auto color = buffer.at(x, y);
                    currentPixel[0] = color.r();
                    currentPixel[1] = color.g();
                    currentPixel[2] = color.b();
                    currentPixel[3] = color.a();
                }
                png_write_row(png_ptr, row);
            }
        });
}

void writePng(const std::string& filename, const IntensityBuffer& buffer)
{
    write(filename,
        buffer.width(), buffer.height(),
        PNG_COLOR_TYPE_GRAY, /*stride*/ 1,
        [&buffer](png_structp png_ptr, png_bytep row) {
            for (size_t y = 0; y < buffer.height(); y++) {
                for (size_t x = 0; x < buffer.width(); x++) {
                    row[x] = buffer.at(x, y);
                }
                png_write_row(png_ptr, row);
            }
        });
}

} // namespace glow
