# Dev Notes

- Pixel size.
   
   I use freetype lib for text rasterization. Pixel size in rendering request is the pixel_height in [FT_Set_Pixel_Sizes](https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_set_pixel_sizes)
- How resulted pic size is culculated?
    
    You can find code [here](https://github.com/dekonoplyov/makemeglow/blob/master/src/font_rasterizer.cpp#L78)

- Glow effect
  
  I've implemented two pass Gaussian blur algorithm mentioned [this article](https://learnopengl.com/Advanced-Lighting/Bloom)

- Margin size
  
  It's computed automatically. For text without glowing it equals to **0**. It equals to **kernel_size / 2** for rendering with blur.

- Testing

  Lib was tested on 
  - Ubuntu 18.04.3, gcc 7.4.0 & clang 6.0.0, cmake 3.10.2
  - Ubuntu 16.04.6, gcc 5.4.0, cmake 3.5.1
  - Mac OS 10.14.5, Apple LLVM version 10.0.1, cmake version 3.15.2


