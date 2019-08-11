#include "makemeglow/png.h"

#include <png.h>

#include <functional>
#include <stdexcept>

namespace glow {

namespace {

constexpr int BIT_DEPTH = 8;
constexpr int GRAY_STRIDE = 1;
constexpr int RGB_STRIDE = 3;

enum class PngIOMode {
    Read,
    Write
};

class LibPngIOGuard {
public:
    LibPngIOGuard(PngIOMode mode,
        FILE* file,
        png_structpp png,
        png_infopp info,
        png_bytepp row)
        : mode_{mode}
        , file_{file}
        , png_{png}
        , info_{info}
        , row_{row}
    {
    }
    ~LibPngIOGuard()
    {
        if (file_ != nullptr) {
            fclose(file_);
        }

        if (*png_ != nullptr) {
            if (*info_ != nullptr) {
                png_destroy_info_struct(*png_, info_);
            }

            switch (mode_) {
            case PngIOMode::Read:
                png_destroy_read_struct(png_, nullptr, nullptr);
                break;
            case PngIOMode::Write:
                png_destroy_write_struct(png_, nullptr);
                break;
            }
        }

        delete[](*row_);
    }

public:
    PngIOMode mode_;
    FILE* file_;
    png_structpp png_;
    png_infopp info_;
    png_bytepp row_;
};

std::string toString(PngIOMode mode)
{
    switch (mode) {
    case PngIOMode::Read:
        return "rb";
    case PngIOMode::Write:
        return "wb";
    }
    throw std::runtime_error{"Unknown io mode"};
}

class LibPngIOWrapper {
public:
    LibPngIOWrapper(const std::string& filename, PngIOMode mode)
        : file{nullptr}
        , png{nullptr}
        , info{nullptr}
        , row{nullptr}
        , guard{mode, file, &png, &info, &row}
    {
        file = fopen(filename.c_str(), toString(mode).c_str());
        if (file == nullptr) {
            throw std::runtime_error{"Could not open file"};
        }

        switch (mode) {
        case PngIOMode::Read:
            png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            break;
        case PngIOMode::Write:
            png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            break;
        }
        if (png == nullptr) {
            throw std::runtime_error{"Could not allocate png struct"};
        }

        info = png_create_info_struct(png);
        if (info == nullptr) {
            throw std::runtime_error{"Could not allocate png info struct"};
        }

        // libPNG exception handling
        if (setjmp(png_jmpbuf(png))) {
            throw std::runtime_error{"Error during png creation"};
        }
    }

public:
    FILE* file;
    png_structp png;
    png_infop info;
    png_bytep row;
    LibPngIOGuard guard;
};

void write(const std::string& filename,
    int width, int height,
    int colorMode, int stride,
    std::function<void(png_structp png, png_bytep)> writeCallback)
{
    LibPngIOWrapper writer{filename, PngIOMode::Write};
    png_init_io(writer.png, writer.file);
    png_set_IHDR(writer.png, writer.info,
        width, height,
        BIT_DEPTH,
        colorMode, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(writer.png, writer.info);

    writer.row = new png_byte[stride * width];
    writeCallback(writer.png, writer.row);

    png_write_end(writer.png, nullptr);
}

template <typename T>
Buffer<T> read(const std::string& filename,
    int colorMode, int stride,
    std::function<T(png_bytep)> readPixel)
{
    LibPngIOWrapper reader{filename, PngIOMode::Read};
    png_init_io(reader.png, reader.file);
    png_read_info(reader.png, reader.info);

    if (png_get_bit_depth(reader.png, reader.info) != BIT_DEPTH) {
        throw std::runtime_error{"Unsupported bit depth"};
    }

    if (png_get_color_type(reader.png, reader.info) != colorMode) {
        throw std::runtime_error{"Unsupported color mode"};
    }

    const auto width = png_get_image_width(reader.png, reader.info);
    const auto height = png_get_image_height(reader.png, reader.info);
    Buffer<T> buffer{width, height};
    reader.row = new png_byte[stride * width];
    for (size_t y = 0; y < height; y++) {
        png_read_row(reader.png, reader.row, nullptr);
        for (size_t x = 0; x < width; ++x) {
            buffer(x, y) = readPixel(&reader.row[x * stride]);
        }
    }

    return buffer;
}

} // namespace

void writePng(const std::string& filename, const ColorBuffer& buffer)
{
    write(filename,
        buffer.width(), buffer.height(),
        PNG_COLOR_TYPE_RGB, RGB_STRIDE,
        [&buffer](png_structp png, png_bytep row) {
            for (size_t y = 0; y < buffer.height(); y++) {
                for (size_t x = 0; x < buffer.width(); x++) {
                    png_byte* currentPixel = &row[x * RGB_STRIDE];
                    const auto color = buffer(x, y);
                    currentPixel[0] = color.r();
                    currentPixel[1] = color.g();
                    currentPixel[2] = color.b();
                }
                png_write_row(png, row);
            }
        });
}

void writePng(const std::string& filename, const IntensityBuffer& buffer)
{
    write(filename,
        buffer.width(), buffer.height(),
        PNG_COLOR_TYPE_GRAY, GRAY_STRIDE,
        [&buffer](png_structp png, png_bytep row) {
            for (size_t y = 0; y < buffer.height(); y++) {
                for (size_t x = 0; x < buffer.width(); x++) {
                    row[x] = buffer(x, y);
                }
                png_write_row(png, row);
            }
        });
}

IntensityBuffer readGrayPng(const std::string& filename)
{
    return read<uint8_t>(filename,
        PNG_COLOR_TYPE_GRAY, GRAY_STRIDE,
        [](png_bytep pixel) { return *pixel; });
}

ColorBuffer readRGBPng(const std::string& filename)
{
    return read<Color>(filename,
        PNG_COLOR_TYPE_RGB, RGB_STRIDE,
        [](png_bytep pixel) {
            return Color{
                pixel[0],
                pixel[1],
                pixel[2]};
        });
}

} // namespace glow
