# vega16
Welcome to the VEGA16 game engine, with this engine there's included a simple video game called VEGA1! 

If you're here already, you probably want to download and use this engine, the thing is how are you gonna do that without compiling it?!

1. Download the latest release from releases
2. Just compile it, it's really easy

How the hell do I compile it?

WINDOWS

1. Download MSYS2 https://www.msys2.org/
2. Install it (capitan obvious)
3. Open up MSYS2 MINGW64
4. Install GCC/G++, it's really simple, just type in this command `pacman -S mingw-w64-x86_64-gcc`
5. Now install SDL2, really simple as well, just `pacman -S mingw-w64-x86_64-SDL2`
6. Now clone the repository by doing `git clone github.com/nebulatarxz/vega16`
7. CD into the folder (capitan obvious again) by doing `cd vega16`
8. If you want to compile the engine with the game do `g++ main.cpp VEGA1.cpp -o vega16 -I/mingw64/include -Dmain=SDL_main -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -static-libgcc -static-libstdc++ -mconsole -Wl,-subsystem,console`, otherwise do `g++ main.cpp -o vega16 -I/mingw64/include -Dmain=SDL_main -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -static-libgcc -static-libstdc++ -mconsole -Wl,-subsystem,console`
9. And run `./vega16`

ARCH

1. Type in this command 'sudo pacman -S `sdl2 base-devel`
2. Clone the repository by doing `git clone github.com/nebulatarxz/vega16`
3. CD into the folder (capitan obvious arch edition) by doing `cd vega16`
4. If you want to compile the engine with the game do `g++ main.cpp VEGA1.cpp -o vega16 -lSDL2main -lSDL2 -static-libgcc -static-libstdc++ -mconsole -Wl,-subsystem,console`, otherwise do `g++ main.cpp -o vega16  -lSDL2main -lSDL2 -static-libgcc -static-libstdc++ -mconsole -Wl,-subsystem,console`
5. And run `./vega16`

More tutorials coming soon, or you can just figure out how to install g++ and sdl2 on your OS, just google it
