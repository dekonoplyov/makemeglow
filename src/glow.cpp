#include "makemeglow/glow.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <cstdint>
#include <exception>
#include <iostream>

namespace glow {

#define WIDTH 640
#define HEIGHT 480

/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];

/* Replace this function with something useful. */

void draw_bitmap(FT_Bitmap* bitmap,
    FT_Int x,
    FT_Int y)
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

            image[j][i] |= bitmap->buffer[q * bitmap->width + p];
        }
    }
}

void show_image(void)
{
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            putchar(image[i][j] == 0 ? ' '
                                     : image[i][j] < 128 ? '+'
                                                         : '*');
        putchar('\n');
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

    std::cout << face->units_per_EM << " " << face->num_glyphs << '\n';

    auto error = FT_Set_Char_Size(
        face, /* handle to face object           */
        0, /* char_width in 1/64th of points  */
        16 * 64, /* char_height in 1/64th of points */
        300, /* horizontal device resolution    */
        300); /* vertical device resolution      */

    // char charcode{'a'};
    // auto glyph_index = FT_Get_Char_Index(face, charcode);

    // const auto load_flags{FT_LOAD_DEFAULT};
    // error = FT_Load_Glyph(
    //     face, /* handle to face object */
    //     glyph_index, /* glyph index           */
    //     load_flags); /* load flags, see below */
    // if (error != 0) {
    //     throw std::runtime_error{"failed to load glyph"};
    // }

    FT_GlyphSlot slot = face->glyph; /* a small shortcut */
    int pen_x = 300;
    int pen_y = 200;

    pen_x = 300;
    pen_y = 200;

    const std::string text{"abcd"};

    for (const auto c : text) {
        FT_UInt glyph_index;

        /* retrieve glyph index from character code */
        glyph_index = FT_Get_Char_Index(face, c);

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
        if (error)
            continue; /* ignore errors */

        /* now, draw to our target surface */
        draw_bitmap(&slot->bitmap,
            pen_x + slot->bitmap_left,
            pen_y - slot->bitmap_top);

        /* increment pen position */
        pen_x += slot->advance.x >> 6;
        pen_y += slot->advance.y >> 6; /* not useful for now */
    }

    show_image();

    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

int foo()
{
    some();

    return 42;
}

} // namespace glow
