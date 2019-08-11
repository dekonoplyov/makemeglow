#include "makemeglow/font_rasterizer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <stdexcept>

namespace glow {

namespace {

struct FTLibGuard {
    FTLibGuard(FT_Library* library)
        : library_{library}
    {
    }

    ~FTLibGuard()
    {
        if (library_ != nullptr) {
            FT_Done_FreeType(*library_);
        }
    }

    FT_Library* library_;
};

void initFTLibrary(FT_Library* library)
{
    if (FT_Init_FreeType(library) != 0) {
        throw std::runtime_error{"Failed to init freetype library"};
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
            "The font file could be opened and read, but font format is unsupported"};
    } else if (error != 0) {
        throw std::runtime_error{
            "The font file could not be opened or read, or that it is broken"};
    }
}

void checkScalability(FT_Face face)
{
    if (FT_IS_SCALABLE(face) == 0) {
        throw std::runtime_error{"Non scalable fonts aren't supported"};
    }
}

void loadGlyph(FT_Face face, char c)
{
    if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0) {
        throw std::runtime_error{"Failed to load glyph"};
    }
}

FT_Pos getKerning(FT_Face face, FT_UInt previous, FT_UInt glyphIndex)
{
    FT_Vector delta;
    FT_Get_Kerning(face, previous, glyphIndex, FT_KERNING_DEFAULT, &delta);
    return delta.x >> 6;
}

struct FontRasterizerInfo {
    size_t bufferWidth;
    size_t bufferHeight;
    FT_Int maxBearingY;
};

FontRasterizerInfo preprocessText(FT_Face face, const std::string& text)
{
    // Compute wrapping buffer size
    // lastCharXTail = width - advance
    // width = sum(advance.x + kerning) + lastCharXTail
    // height = maxBearingY + maxTailY
    // Also remember maxBearingY it's our baseline
    FT_Pos width = 0;
    FT_Int maxBearingY = 0;
    FT_Int maxTailY = 0;
    FT_Int lastCharTailX = 0;

    const FT_Bool useKerning = FT_HAS_KERNING(face);
    FT_UInt previousGlyphIndex = 0;

    for (const auto c : text) {
        loadGlyph(face, c);
        auto slot = face->glyph;

        const auto glyphIndex = FT_Get_Char_Index(face, c);
        if (useKerning && previousGlyphIndex != 0 && glyphIndex != 0) {
            width += getKerning(face, previousGlyphIndex, glyphIndex);
        }
        const auto advance = slot->advance.x >> 6;
        width += advance;
        lastCharTailX = static_cast<FT_Int>(slot->bitmap.width) - advance;

        maxBearingY = std::max(maxBearingY, slot->bitmap_top);

        const auto tailY = static_cast<FT_Int>(slot->bitmap.rows) - slot->bitmap_top;
        maxTailY = std::max(maxTailY, tailY);

        // update glyphIndex for kerning
        previousGlyphIndex = glyphIndex;
    }

    return {
        static_cast<size_t>(width + lastCharTailX),
        static_cast<size_t>(maxBearingY + maxTailY),
        maxBearingY};
}

void drawBitamp(IntensityBuffer& buffer, size_t left, size_t top, FT_Bitmap* bitmap)
{
    for (size_t y = 0; y < bitmap->rows; ++y) {
        for (size_t x = 0; x < bitmap->width; ++x) {
            buffer(left + x, top + y) = std::max(
                buffer(left + x, top + y),
                bitmap->buffer[y * bitmap->width + x]);
        }
    }
}

} // namespace

class FontRasterizer : public FontRasterizerInterface {
public:
    FontRasterizer(const std::string& font)
        : library_{nullptr}
        , guard_{&library_}
        , face_{nullptr}
    {
        initFTLibrary(&library_);
        loadFont(&library_, &face_, font);
        checkScalability(face_);
    }

    IntensityBuffer rasterize(const std::string& text, size_t pixelSize, size_t margin = 0) override
    {
        if (FT_Set_Pixel_Sizes(face_, 0, pixelSize) != 0) {
            throw std::runtime_error{"Failed to set pixel size"};
        }

        const auto rasterInfo = preprocessText(face_, text);
        IntensityBuffer buffer{
            rasterInfo.bufferWidth + 2 * margin,
            rasterInfo.bufferHeight + 2 * margin};

        auto left = static_cast<FT_Pos>(margin);

        const FT_Bool hasKerning = FT_HAS_KERNING(face_);
        FT_UInt previousGlyphIndex = 0;

        for (const auto c : text) {
            const auto glyphIndex = FT_Get_Char_Index(face_, c);
            if (hasKerning && previousGlyphIndex != 0 && glyphIndex != 0) {
                left += getKerning(face_, previousGlyphIndex, glyphIndex);
            }

            loadGlyph(face_, c);
            const auto slot = face_->glyph;

            FT_Int top = margin + rasterInfo.maxBearingY - face_->glyph->bitmap_top;
            if (top < static_cast<FT_Int>(margin)) {
                top = margin;
            }
            drawBitamp(buffer,
                static_cast<size_t>(left), static_cast<size_t>(top),
                &slot->bitmap);

            left += slot->advance.x >> 6;

            // update glyphIndex for kerning
            previousGlyphIndex = glyphIndex;
        }

        return buffer;
    }

private:
    FT_Library library_;
    FTLibGuard guard_;
    FT_Face face_;
};

FontRasterizerPtr makeFontRasterizer(const std::string& font)
{
    return std::make_unique<FontRasterizer>(font);
}

} // namespace glow
