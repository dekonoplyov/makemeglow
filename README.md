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
You can use glow app and listen [Lenny's masterpiece](https://www.youtube.com/watch?v=ucvLuGgsGS8).
```
./glow_app -f ../../tests/data/font/JosefinSans-Regular.ttf -s 40 I belong to you
```
![glow_app1](data/glow_app1.png "glow_app1")

Chose different font, size and specify colors
```
./glow_app -f ../../tests/data/font/IndieFlower-Regular.ttf -s 30 -c f0ff00 -b 100050 -- And you you belong to me too
```
![glow_app2](data/glow_app2.png "glow_app2")

Set Gauss kernel params
```
./glow_app -f ../../tests/data/font/NotoSansJP-Black.otf -s 30 -c 0fff9f --kernel 15 --sigma 15.0 -- You make my life complete
```
![glow_app3](data/glow_app3.png "glow_app3")

Specify output
```
./glow_app -f ../../tests/data/font/NotoSansJP-Black.otf -o ./path_to_text.png -s 30 -b 55124f  -- You make me feel so sweet
```
![glow_app4](data/glow_app4.png "glow_app4")

For full help use
```
./glow_app -h
```

Note, that I use cxxopts to parse command line input, and it can skip some chars, like comas, quote marks, etc.
If you have some problems, just paste your string [here](https://github.com/dekonoplyov/makemeglow/blob/master/examples/hello.cpp#L8)

# Requirements
Compiler with C++14 support.

[Libpng](http://www.libpng.org/pub/png/libpng.html) and [Freetype](https://www.freetype.org/)

Install on Linux
```
sudo apt-get install libfreetype6-dev
sudo apt-get install libpng-dev
```
To get freetype ppa use one of these links [1](http://ubuntuhandbook.org/index.php/2017/06/install-freetype-2-8-in-ubuntu-16-04-17-04/)[2](http://ubuntuhandbook.org/index.php/2016/09/install-freetype-2-7-ubuntu-16-04/)

Install on Mac
```
brew install libpng
brew install freetype
```

Lib also uses gtest and cxxopts, but they're git submodules, so you shouldn't worry.

# Notes
You can look at detailed dev notes [here](docs/notes.md)

# TODO list
- png read/write with different bitdepth and color modes
- fixed size fonts
- support for alpha channel in text and background color
- non-ASCII encodings
- multiple Gauss kernel pass
- stable approach to test font rasterization
