// Version: 0.0.4
// Credits:
// @skyliuswastaken on Discord: Main Developer
// @_na69_ on Discord: Engine Contributor
// @traox_ on Discord: Engine Contributor & Main Designer

#define SDL_MAIN_HANDLED
#include "UCS.h"
#include <iostream>

int main(int argc, char* argv[]) {
    VEGA16 game;

    if (!game.init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return -1;
    }

    game.run();

    return 0;
}
