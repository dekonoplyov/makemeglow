#include "font_rasterizer.h"

#include <iostream>
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

void loadGlyph(FT_Face* face, char c)
{
    if (FT_Load_Char(*face, c, FT_LOAD_RENDER) != 0) {
        throw std::runtime_error{"failed to load glyph"};
    }
}

void drawBitamp(ColorBuffer* buffer, size_t left, size_t top, FT_Bitmap* bitmap)
{
    for (size_t y = 0; y < bitmap->rows; ++y) {
        for (size_t x = 0; x < bitmap->width; ++x) {
            buffer->set(left + x, top + y,
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

ColorBuffer FontRasterizer::createPropperSizeBuffer(const std::string& text)
{
    // TODO cleanup converions + refactor
    FT_Pos width = 0;
    int max_height = 0;
    for (const auto c : text) {
        loadGlyph(&face_, c);
        width += face_->glyph->advance.x >> 6;
    
        FT_Bitmap* bitmap = &(face_->glyph->bitmap);
        int top = (face_->ascender >> 6) - face_->glyph->bitmap_top;
        if (top < 0)
            top = 0;
        max_height = std::max(max_height, static_cast<int>(top + bitmap->rows));
    }

    return ColorBuffer{static_cast<size_t>(width), static_cast<size_t>(max_height)};
}

ColorBuffer FontRasterizer::rasterize()
{
    const size_t pixelSize = 40;
    if (FT_Set_Pixel_Sizes(face_, 0, pixelSize) != 0) {
        throw std::runtime_error{"failed to set pixel sizes glyph"};
    }

    const std::string text{"abdcde fg.,<>(){}!@#$%^&*\"'{}}|\\/"};
    auto buffer = createPropperSizeBuffer(text);

    size_t left = 0;
    for (const auto c : text) {
        loadGlyph(&face_, c);

        FT_GlyphSlot slot = face_->glyph;

        int top = (face_->ascender >> 6) - face_->glyph->bitmap_top;
        if (top < 0) {
            top = 0;
        }
        drawBitamp(&buffer, left, top, &slot->bitmap);

        left += slot->advance.x >> 6;
    }

    return buffer;
}

} // namespace glow
