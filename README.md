# Untitled Cat Shooter (VEGA16)

Welcome to **Untitled Cat Shooter**, a shitty video game made by a bunch of european retards!

## Getting Started
If you're here, you probably want to download and play the game. To do that you needa compile it yk, so here's how

### Download the Latest Release
1. Head over to the **Releases** section on GitHub.
2. Download the latest version.

---
## Compilation Guide

> [!CAUTION]
> **Note:** Linux IS no longer supported, the development team really tried but none of us use Linux so we cannot test it, you're on your own, the only Linux support that will be kept is the Arch Linux tutorial.

### Windows (MSYS2)
1. Download and install [MSYS2](https://www.msys2.org/).
2. Open **MSYS2 MINGW64**.
3. Install GCC/G++ by running:

   ```sh
   pacman -S mingw-w64-x86_64-gcc
   ```
4. Install SDL2 with:

   ```sh
   pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_gfx
   ```
5. Clone the repository:

   ```sh
   git clone https://github.com/nebulatarxz/v16ucs
   ```
6. Navigate into the folder:

   ```sh
   cd v16ucs
   ```
7. Compile the engine using one of the following methods:

   - **Manual Compilation:**
     ```sh
     g++ main.cpp UCS.cpp resource.o -o UntitledCatShooter -I/mingw64/include -I/mingw64/include/SDL2 -Dmain=SDL_main -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2_ttf -lSDL2_gfx -lSDL2 -lSDL2_image -mconsole -Wl,-subsystem,console -fpermissive -std=c++11
     ```

   - **Using Make:**
     ```sh
     make
     ```
8. Run the game:

   ```sh
   ./bin/UntitledCatShooter
   ```

---
### Arch Linux
1. Install dependencies:

   ```sh
   sudo pacman -S sdl2 sdl2_ttf sdl2_gfx base-devel make
   ```
2. Clone the repository:

   ```sh
   git clone https://github.com/nebulatarxz/v16ucs
   ```
3. Navigate into the folder:

   ```sh
   cd v16ucs
   ```
4. Compile the engine using one of the following methods:

   - **Manual Compilation:**
     ```sh
     g++ main.cpp UCS.cpp resource.o -o UntitledCatShooter -Dmain=SDL_main -lSDL2main -lSDL2_ttf -lSDL2_gfx -lSDL2 -lSDL2_image -Wl,-subsystem,console -fpermissive -std=c++11
     ```

   - **Using Make:**
     ```sh
     make
     ```
5. Run the game:

   ```sh
   ./bin/UntitledCatShooter
   ```

---
> [!TIP]
> To modify the game, simply edit **UCS.cpp**.

## Need More Help?
More tutorials will be available soon! If you're using a different OS, you can look up how to install **g++** and **SDL2** for your system.

Enjoy the game or the engine!
