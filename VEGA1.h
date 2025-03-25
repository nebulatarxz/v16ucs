#ifndef VEGA1_H
#define VEGA1_H
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BLOCK_SIZE = 50;

class VEGA16 {
public:
    VEGA16();  
    bool init();
    void handleEvents();
    void update();
    void render();
    void displayMessage(const char* message, int yOffset);
    void restartGame();
    void clean();
    bool running;
    void drawTree(int x, int y);
    void checkTreeCollisions();
    int prevPlayerX, prevPlayerY; 
    void displayMessage(const std::string& message, int yOffset);

private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect player;
    TTF_Font* font;
    bool isCrushed;
    bool isDoubleJumping;    
    int jumpHeight;           
    int frameCount;
    int fps;
    Uint32 lastTime;
    Uint32 jumpTimer;          
};

#endif
