#ifndef UCS_H
#define UCS_H
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <algorithm>



const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BLOCK_SIZE = 50;
struct Bullet {
    SDL_Rect rect;
    bool active;
};


class VEGA16 {
public:
    SDL_Texture* gunTexture; 
    std::vector<Bullet> bullets; 
    void drawTree(int x, int y, int damage);

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
	SDL_Texture* catTexture;
    void checkTreeCollisions();
    int prevPlayerX, prevPlayerY;
    void displayMessage(const std::string& message, int yOffset);

private:
    std::vector<int> treeDamage;
    int catWidth, catHeight;
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
