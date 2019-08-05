#include "font_rasterizer.h"

#include <algorithm>
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

FT_Pos getKerning(FT_Face face, FT_UInt previous, FT_UInt glyphIndex)
{
    FT_Vector delta;
    FT_Get_Kerning(face, previous, glyphIndex, FT_KERNING_DEFAULT, &delta);
    return delta.x >> 6;
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

    // TODO check if it could be reduced
    int lastCharXTail = 0;

    const FT_Bool useKerning = FT_HAS_KERNING(face);
    FT_UInt previous = 0;

    for (const auto c : text) {
        loadGlyph(face, c);
        auto slot = face->glyph;

        const auto glyphIndex = FT_Get_Char_Index(face, c);
        if (useKerning && previous != 0 && glyphIndex != 0) {
            width += getKerning(face, previous, glyphIndex);
        }

        const FT_Pos tail = static_cast<int>(slot->bitmap.rows) - slot->bitmap_top;

        const auto advance = slot->advance.x >> 6;
        width += advance;
        lastCharXTail = slot->bitmap.rows - advance;

        maxTail = std::max(maxTail, tail);
        maxBearingY = std::max(maxBearingY, slot->bitmap_top);

        previous = glyphIndex;
    }

    if (lastCharXTail < 0) {
        lastCharXTail = 0;
    }

    return {static_cast<size_t>(width + lastCharXTail),
        static_cast<size_t>(maxBearingY + maxTail),
        static_cast<size_t>(maxBearingY)};
}

void drawBitamp(IntensityBuffer* buffer, size_t left, size_t top, FT_Bitmap* bitmap)
{
    for (size_t y = 0; y < bitmap->rows; ++y) {
        for (size_t x = 0; x < bitmap->width; ++x) {
            buffer->at(left + x, top + y) = std::max(
                buffer->at(left + x, top + y),
                static_cast<uint8_t>(bitmap->buffer[y * bitmap->width + x]));
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

IntensityBuffer FontRasterizer::rasterize(const std::string& text, size_t pixelSize)
{
    if (FT_Set_Pixel_Sizes(face_, 0, pixelSize) != 0) {
        throw std::runtime_error{"failed to set pixel sizes glyph"};
    }

    const FT_Bool hasKerning = FT_HAS_KERNING(face_);
    FT_UInt previous = 0;

    const auto rasterInfo = getInfo(face_, text);
    IntensityBuffer buffer{rasterInfo.bufferWidth, rasterInfo.bufferHeight};

    size_t left = 0;
    for (const auto c : text) {

        const auto glyph_index = FT_Get_Char_Index(face_, c);
        if (hasKerning && previous != 0 && glyph_index != 0) {
            // firs kernig never should be less than zero
            left += getKerning(face_, previous, glyph_index);
        }

        loadGlyph(face_, c);

        const auto slot = face_->glyph;

        int top = rasterInfo.maxBearingY - face_->glyph->bitmap_top;
        if (top < 0) {
            top = 0;
        }
        drawBitamp(&buffer, left, top, &slot->bitmap);

        left += slot->advance.x >> 6;

        previous = glyph_index;
    }

    return buffer;
}

} // namespace glow
