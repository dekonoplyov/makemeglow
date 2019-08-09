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
Compiler with C++14 support.

[Libpng](http://www.libpng.org/pub/png/libpng.html) and [Freetype](https://www.freetype.org/)

Install on Linux
```
sudo apt-get install libpng-dev
```
To install freetype use one of these links [1](http://ubuntuhandbook.org/index.php/2017/06/install-freetype-2-8-in-ubuntu-16-04-17-04/)[2](http://ubuntuhandbook.org/index.php/2016/09/install-freetype-2-7-ubuntu-16-04/)

Install on Mac
```
brew install libpng
brew install freetype
```

Lib also uses gtest and cxxopts, but they're git submodules, so you shouldn't worry.

# TODO list

