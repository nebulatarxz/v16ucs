#include "VEGA1.h"  

VEGA16::VEGA16() : running(false), window(nullptr), renderer(nullptr) {
    player = {SCREEN_WIDTH / 2 - BLOCK_SIZE / 2, SCREEN_HEIGHT / 2 - BLOCK_SIZE / 2, BLOCK_SIZE, BLOCK_SIZE};
}

bool VEGA16::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("VEGA¹, the official game for the VEGA¹⁶ video game engine!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    running = true;
    return true;
}


void VEGA16::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            const int moveSpeed = 5; 
            const int screen_width_limit = SCREEN_WIDTH - BLOCK_SIZE;
            const int screen_height_limit = SCREEN_HEIGHT - BLOCK_SIZE;

            if (event.key.keysym.sym == SDLK_w && player.y > 0) {
                player.y -= moveSpeed;  
            }
            if (event.key.keysym.sym == SDLK_s && player.y < screen_height_limit) {
                player.y += moveSpeed; 
            }
            if (event.key.keysym.sym == SDLK_a && player.x > 0) {
                player.x -= moveSpeed; 
            }
            if (event.key.keysym.sym == SDLK_d && player.x < screen_width_limit) {
                player.x += moveSpeed;  
            }
        }
    }
}

void VEGA16::update() {
}

void VEGA16::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    SDL_Rect blockRect = {player.x, player.y, player.w, player.h};
    SDL_RenderFillRect(renderer, &blockRect);  

    SDL_RenderPresent(renderer); 
}

void VEGA16::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
