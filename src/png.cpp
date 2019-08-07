#include "makemeglow/png.h"

#include <png.h>

#include <functional>
#include <stdexcept>

namespace glow {

namespace {

class LibPngWriteWrapper {
public:
    LibPngWriteWrapper(const std::string& filename)
        : file{nullptr}
        , png{nullptr}
        , info{nullptr}
        , row{nullptr}
    {
        file = fopen(filename.c_str(), "wb");
        if (file == nullptr) {
            throw std::runtime_error{"Could not open file for writing"};
        }

        png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (png == nullptr) {
            throw std::runtime_error{"Could not allocate write struct"};
        }

        info = png_create_info_struct(png);
        if (info == nullptr) {
            throw std::runtime_error{"Could not allocate info struct"};
        }

        // libPNG exception handling
        if (setjmp(png_jmpbuf(png))) {
            throw std::runtime_error{"Error during png creation"};
        }
    }

    ~LibPngWriteWrapper()
    {
        if (file != nullptr) {
            fclose(file);
        }
        if (info != nullptr) {
            png_destroy_info_struct(png, &info);
        }
        if (png != nullptr) {
            png_destroy_write_struct(&png, nullptr);
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

void write(const std::string& filename,
    int width, int height,
    int colorMode, int stride,
    std::function<void(png_structp png, png_bytep)> writeCallback)
{
    LibPngWriteWrapper writer{filename};
    png_init_io(writer.png, writer.file);

    png_set_IHDR(writer.png, writer.info,
        width, height,
        /*bit depth*/ 8,
        colorMode, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(writer.png, writer.info);

    writer.row = (png_bytep)malloc(stride * width * sizeof(png_byte));
    writeCallback(writer.png, writer.row);

    png_write_end(writer.png, nullptr);
}

} // namespace

void writePng(const std::string& filename, const ColorBuffer& buffer)
{
    write(filename,
        buffer.width(), buffer.height(),
        PNG_COLOR_TYPE_RGBA, /*stride*/ 4,
        [&buffer](png_structp png, png_bytep row) {
            for (size_t y = 0; y < buffer.height(); y++) {
                for (size_t x = 0; x < buffer.width(); x++) {
                    png_byte* currentPixel = &row[x * 4];
                    const auto color = buffer.at(x, y);
                    currentPixel[0] = color.r();
                    currentPixel[1] = color.g();
                    currentPixel[2] = color.b();
                    currentPixel[3] = color.a();
                }
                png_write_row(png, row);
            }
        });
}

void writePng(const std::string& filename, const IntensityBuffer& buffer)
{
    write(filename,
        buffer.width(), buffer.height(),
        PNG_COLOR_TYPE_GRAY, /*stride*/ 1,
        [&buffer](png_structp png, png_bytep row) {
            for (size_t y = 0; y < buffer.height(); y++) {
                for (size_t x = 0; x < buffer.width(); x++) {
                    row[x] = buffer.at(x, y);
                }
                png_write_row(png, row);
            }
        });
}

} // namespace glow
