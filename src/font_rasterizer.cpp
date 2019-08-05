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

void loadFont(FT_Library* library, FT_Face* face, const std::string& font)
{
    const auto error = FT_New_Face(*library,
        font.c_str(),
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

void loadGlyph(FT_Face face, char c)
{
    if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0) {
        throw std::runtime_error{"failed to load glyph"};
    }
}

struct RasterizerInfo {
    size_t bufferWidth;
    size_t bufferHeight;
    size_t maxBearingY;
};

RasterizerInfo getInfo(FT_Face face, const std::string& text)
{
    // TODO refactor types and conversions
    // compute max from baseline -- max of all tops
    // compute max tail -- max of diffs rows and tops
    FT_Pos width = 0;
    int maxBearingY = 0;
    FT_Pos maxTail = 0;

    for (const auto c : text) {
        loadGlyph(face, c);
        auto slot = face->glyph;

        FT_Pos tail = static_cast<int>(slot->bitmap.rows) - slot->bitmap_top;

        width += slot->advance.x >> 6;
        maxTail = std::max(maxTail, tail);
        maxBearingY = std::max(maxBearingY, slot->bitmap_top);
    }

    return {static_cast<size_t>(width),
        static_cast<size_t>(maxBearingY + maxTail),
        static_cast<size_t>(maxBearingY)};
}

void drawBitamp(ColorBuffer* buffer, size_t left, size_t top, FT_Bitmap* bitmap)
{
    for (size_t y = 0; y < bitmap->rows; ++y) {
        for (size_t x = 0; x < bitmap->width; ++x) {
            // TODO check better way to remove black borders
            // TODO make pretty or operator
            const auto intensity = std::max(
                buffer->at(left + x, top + y).r(),
                static_cast<uint8_t>(bitmap->buffer[y * bitmap->width + x]));

            buffer->set(left + x, top + y,
                Color{intensity, intensity, intensity, intensity});
        }
    }
}

} // namespace

struct FontRasterizerOptions {
    std::string font;
    size_t size;
};

FontRasterizer::FontRasterizer(const std::string& font)
{
    initFTLibrary(&library_);
    loadFont(&library_, &face_, font);
}

FontRasterizer::~FontRasterizer()
{
    FT_Done_Face(face_);
    FT_Done_FreeType(library_);
}

ColorBuffer FontRasterizer::rasterize(size_t pixelSize)
{
    if (FT_Set_Pixel_Sizes(face_, 0, pixelSize) != 0) {
        throw std::runtime_error{"failed to set pixel sizes glyph"};
    }

    const std::string text{"abdcde fg.,<>(){}!@#$%^&*\"'{}}|\\/A"};
    const auto rasterInfo = getInfo(face_, text);
    ColorBuffer buffer{rasterInfo.bufferWidth, rasterInfo.bufferHeight};

    size_t left = 0;
    for (const auto c : text) {
        loadGlyph(face_, c);

        FT_GlyphSlot slot = face_->glyph;

        int top = rasterInfo.maxBearingY - face_->glyph->bitmap_top;
        if (top < 0) {
            top = 0;
        }
        drawBitamp(&buffer, left, top, &slot->bitmap);

        left += slot->advance.x >> 6;
    }

    return buffer;
}

} // namespace glow
