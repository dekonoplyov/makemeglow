# Dev Notes

- Pixel size.
   
   I use freetype lib for text rasterization. Pixel size in rendering request is the pixel_height in [FT_Set_Pixel_Sizes](https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_set_pixel_sizes)
- How resulted pic size is culculated?
    
    Text is preprocessed before rasterization to compture buffer sizes. X size is equals to the sum of each glyph advance and it's kerning plus X tail for last char. X tail for last char is a num of it's columns minus advance.


- Glow effect

- Margin size
- Png comparison in tests

