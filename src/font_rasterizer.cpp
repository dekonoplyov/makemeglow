#include "font_rasterizer.h"

#include <stdexcept>

namespace glow {

namespace {

void initFTLibrary(FT_Library* library)
{
    const auto error = FT_Init_FreeType(library);
    if (error != 0) {
        throw std::runtime_error{"failed to init library "};
    }
}

void loadFont(FT_Library* library, FT_Face* face)
{
    const auto error = FT_New_Face(*library,
        "/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf",
        /*face_index*/ 0,
        face);
    if (error == FT_Err_Unknown_File_Format) {
        throw std::runtime_error{
            "the font file could be opened and read, but font format is unsupported"};
    } else if (error != 0) {
        throw std::runtime_error{
            "the font file could not be opened or read, or that it is broken"};
    }
}

void drawBitamp(ColorBuffer* buffer, FT_Bitmap* bitmap, FT_Int xOffset)
{
    for (int y = 0; y < bitmap->rows; y++) {
        for (int x = xOffset; x < xOffset + bitmap->width; x++) {
            buffer->set(x, y,
                Color{static_cast<uint8_t>(bitmap->buffer[y * bitmap->width + x])});
        }
    }
}

} // namespace

struct FontRasterizerOptions {
    std::string font;
    size_t size;
};

FontRasterizer::FontRasterizer()
{
    initFTLibrary(&library_);
    loadFont(&library_, &face_);
}

FontRasterizer::~FontRasterizer()
{
    FT_Done_Face(face_);
    FT_Done_FreeType(library_);
}

ColorBuffer FontRasterizer::rasterize()
{
    if (FT_Set_Pixel_Sizes(face_, 0, 20) != 0) {
        throw std::runtime_error{"failed to load glyph"};
    }

    const std::string text{"abcd"};
    ColorBuffer buffer{
        static_cast<size_t>(face_->max_advance_width) * text.size(),
        static_cast<size_t>(face_->max_advance_height)};


    int xOffset = 0;
    for (const auto c : text) {
        if (FT_Load_Char(face_, c, FT_LOAD_RENDER) != 0) {
            throw std::runtime_error{"failed to load glyph"};
        }

        FT_GlyphSlot slot = face_->glyph;

        /* now, draw to our target surface */
        drawBitamp(&buffer, &slot->bitmap, xOffset);

        /* increment pen position */
        xOffset += slot->advance.x >> 6;
    }

    return buffer;
}

} // namespace glow
