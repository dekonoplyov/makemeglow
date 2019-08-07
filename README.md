![makemeglow logo](data/logo200px.png "makemeglow")
Text rasterizer lib with glowing effect.

# Quick start
## Build

```
mkdir build
cd build
cmake ..
make
```

## Run tests
```
make tests
```

## Render text
You can use glow app.
```
./glow_app -f /path/to/font/file.ttf "positional args are parts of text to render"
```

# Requirements
Compiler with C++17 support.

[Libpng](http://www.libpng.org/pub/png/libpng.html) and [Freetype](https://www.freetype.org/)

Install on Linux
```
sudo apt-get install libpng-dev
sudo add-apt-repository ppa:glasen/freetype2
sudo apt update && sudo apt install freetype2-demos
```

Install on Mac
```
brew install libpng
brew install freetype
```

Lib also uses gtest and cxxopts, but they're git submodules, so you shouldn't worry.

# Requirements


# TODO list

