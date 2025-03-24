#define SDL_MAIN_HANDLED  
#include <SDL2/SDL.h>
#include <iostream>
#include "VEGA16.h"  

int main(int argc, char* argv[]) {
    VEGA16 engine;
    if (!engine.init()) return -1;

    while (engine.running) {
        engine.handleEvents();
        engine.update();
        engine.render();
    }

    engine.clean();
    return 0;
}
