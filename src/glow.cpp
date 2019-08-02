#include "makemeglow/glow.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <png.h>

#include <cstdint>
#include <exception>
#include <iostream>

namespace glow {

#define WIDTH 100
#define HEIGHT 100

/* origin is the upper left corner */
uint8_t image[HEIGHT * WIDTH];

/* Replace this function with something useful. */

void draw_bitmap(FT_Bitmap* bitmap, FT_Int x, FT_Int y)
{
    FT_Int i, j, p, q;
    FT_Int x_max = x + bitmap->width;
    FT_Int y_max = y + bitmap->rows;

    /* for simplicity, we assume that `bitmap->pixel_mode' */
    /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

    for (i = x, p = 0; i < x_max; i++, p++) {
        for (j = y, q = 0; j < y_max; j++, q++) {
            if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT)
                continue;

            image[WIDTH * j + i] |= bitmap->buffer[q * bitmap->width + p];
        }
    }
}

void initFTLibrary(FT_Library* library)
{
    auto error = FT_Init_FreeType(library);
    if (error != 0) {
        throw std::runtime_error{"failed to init library "};
    }
}

void loadFont(FT_Library* library, FT_Face* face)
{
    auto error = FT_New_Face(*library,
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

void some()
{
    FT_Library library;
    FT_Face face;

    initFTLibrary(&library);
    loadFont(&library, &face);

    auto error = FT_Set_Char_Size(
        face, /* handle to face object           */
        0, /* char_width in 1/64th of points  */
        40 * 64, /* char_height in 1/64th of points */
        WIDTH, /* horizontal device resolution    */
        HEIGHT); /* vertical device resolution      */

    FT_GlyphSlot slot = face->glyph; /* a small shortcut */
    int pen_x = 50;
    int pen_y = 15;

    const std::string text{"a"};

    for (const auto c : text) {
        FT_UInt glyph_index;

        /* retrieve glyph index from character code */
        glyph_index = FT_Get_Char_Index(face, c);

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
        if (error != 0) {
            throw std::runtime_error{"failed to load glyph"};
        }

        /* now, draw to our target surface */
        draw_bitmap(&slot->bitmap,
            pen_x + slot->bitmap_left,
            pen_y - slot->bitmap_top);

        /* increment pen position */
        pen_x += slot->advance.x >> 6;
        pen_y += slot->advance.y >> 6; /* not useful for now */
    }

    stbi_write_png("./some.png", WIDTH, HEIGHT, 3, &image, WIDTH);
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

void foo()
{
    some();
}

} // namespace glow
