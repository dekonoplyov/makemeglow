#include "makemeglow/color_buffer.h"

#include <png.h>

#include <stdexcept>

namespace glow {

namespace {

class PngWriter {
public:
    PngWriter(const std::string& filename, const ColorBuffer& buffer)
        : filename_{filename}
        , buffer_{buffer}
        , file_ptr_{nullptr}
        , png_ptr_{nullptr}
        , info_ptr_{nullptr}
        , row_{nullptr}
    {
        file_ptr_ = fopen(filename_.c_str(), "wb");
        if (file_ptr_ == nullptr) {
            throw std::runtime_error{"Could not open file for writing"};
        }

        png_ptr_ = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (png_ptr_ == nullptr) {
            throw std::runtime_error{"Could not allocate write struct"};
        }

        info_ptr_ = png_create_info_struct(png_ptr_);
        if (info_ptr_ == nullptr) {
            throw std::runtime_error{"Could not allocate info struct"};
        }

        // libPNG exception handling
        if (setjmp(png_jmpbuf(png_ptr_))) {
            throw std::runtime_error{"Error during png creation"};
        }
    }

    void write()
    {
        png_init_io(png_ptr_, file_ptr_);

        // Write header (8 bit colour depth)
        png_set_IHDR(png_ptr_, info_ptr_,
            buffer_.width(), buffer_.height(),
            /*bit depth*/ 8,
            PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr_, info_ptr_);

        // Allocate memory for one row (4 bytes per pixel - RGBA)
        row_ = (png_bytep)malloc(4 * buffer_.width() * sizeof(png_byte));

        // Write image data
        for (size_t y = 0; y < buffer_.height(); y++) {
            for (size_t x = 0; x < buffer_.width(); x++) {
                png_byte* currentPixel = &row_[x * 4];
                const auto color = buffer_.at(x, y);
                currentPixel[0] = color.r();
                currentPixel[1] = color.g();
                currentPixel[2] = color.b();
                currentPixel[3] = color.a();
            }
            png_write_row(png_ptr_, row_);
        }

        png_write_end(png_ptr_, nullptr);
    }

    ~PngWriter()
    {
        if (file_ptr_ != nullptr) {
            fclose(file_ptr_);
        }
        if (info_ptr_ != nullptr) {
            png_free_data(png_ptr_, info_ptr_, PNG_FREE_ALL, -1);
        }
        if (png_ptr_ != nullptr) {
            png_destroy_write_struct(&png_ptr_, nullptr);
        }
        if (row_ != nullptr) {
            free(row_);
        }
    }

private:
    const std::string& filename_;
    const ColorBuffer& buffer_;
    FILE* file_ptr_;
    png_structp png_ptr_;
    png_infop info_ptr_;
    png_bytep row_;
};

} // namespace

void writePng(const std::string& filename, const ColorBuffer& buffer)
{
    PngWriter{filename, buffer}.write();
}

} // namespace glow
