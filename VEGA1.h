#ifndef VEGA1_H
#define VEGA1_H

#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;
const int SPEED = 0.001;

class VEGA16 {
public:
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect player;

    VEGA16();
    bool init();
    void handleEvents();
    void update();
    void render();
    void clean();
};

#endif // VEGA1_H
