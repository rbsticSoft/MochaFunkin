# Building
This projects requires these dependencies:
* SDL2
* OpenAL
* libxml2
* libvorbisfile
* Freetype2

Make sure you clone recursively
```
git clone --recursive https://github.com/rbsticSoft/MochaFunkin.git
```
If you've already cloned the repository without ``--recursive`` run ``git submodule update --init --recursive`` instead
## Windows, Mac, Linux
CMake is required.
```
mkdir -p build
cd build
cmake ..
make
./FNFC
```
## Switch
devkitPro is required.
```
(dkp-)pacman -S switch-sdl2 switch-openal-soft switch-libxml2 switch-libvorbis switch-freetype2
make -f Makefile.nx
```