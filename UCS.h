// Version: 0.0.4
// Credits:
// @skyliuswastaken on Discord: Main Developer
// @_na69_ on Discord: Engine Contributor
// @traox_ on Discord: Engine Contributor & Main Designer

#ifndef UCS_H
#define UCS_H
#include <SDL2/SDL.h>

#include <SDL2/SDL_ttf.h>

#include <SDL2/SDL2_gfxPrimitives.h>

#include <SDL2/SDL_image.h>

#include <string>

#include <vector>

#include <memory>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BLOCK_SIZE = 50;
const int FPS_CAP = 60;
const int FRAME_DELAY = 1000 / FPS_CAP;

const int GRAVITY = 1;
const int JUMP_SPEED = 15;
const int MOVE_SPEED = 5;
const int BULLET_SPEED = 10;
const int MAX_JUMP_COUNT = 2;

enum class DebugRenderer {
	NONE,
	COLLISION_BOXES,
	PHYSICS,
	BULLET_PATHS,
	PERFORMANCE,
	PLAYER_STATS,
	GAME_STATE,
	INPUT_DEBUG,
	COLLISION_DEBUG,
	COUNT
};

struct Bullet {
	SDL_Rect rect;
	bool active;
	int direction;
};

struct Tree {
	int x;
	int y;
	int damage;
	SDL_Rect trunk;
	std::vector < SDL_Rect > leaves;
};
class VEGA16 {
public: VEGA16();
	  ~VEGA16();
	  bool init();
	  void run();
	  void clean();
	  void renderPlayerStats();
	  void renderGameState();
private: bool isFacingRight;

	   std::vector < SDL_Keycode > recentInputs;
	   const int INPUT_HISTORY_SIZE = 10;

	   bool showCollisionTests;
	   SDL_Point lastCollisionPoint;

	   bool running;
	   bool isCrushed;

	   SDL_Rect player;
	   int prevPlayerX,
		   prevPlayerY;
	   bool isJumping;
	   int jumpCount;
	   int verticalVelocity;

	   SDL_Window* window;
	   SDL_Renderer* renderer;

	   SDL_Texture* catTexture;
	   SDL_Texture* gunTexture;
	   int catWidth,
		   catHeight;
	   TTF_Font* font;

	   std::vector < Bullet > bullets;
	   std::vector < Tree > trees;

	   Uint32 frameStart;
	   Uint32 lastTime;
	   int frameCount;
	   int fps;

	   bool debugMode;
	   DebugRenderer currentDebugRenderer;

	   void handleEvents();
	   void update();
	   void render();

	   void drawTree(const Tree& tree);
	   void updateTrees();
	   void checkCollisions();
	   void fireBullet();
	   void updateBullets();
	   void displayMessage(const std::string& message, int yOffset);
	   void restartGame();
	   void capFrameRate();

	   void toggleDebugMode();
	   void cycleDebugRenderer(bool forward);
	   void renderDebugInfo();
	   void renderCollisionBoxes();
	   void renderPhysicsInfo();
	   void renderBulletPaths();
	   void renderPerformanceInfo();
	   std::string getDebugRendererName(DebugRenderer renderer);

	   void renderInputDebug();
	   void renderCollisionDebug();
	   void updateInputHistory(SDL_Keycode key);

	   void displayMessage(const std::string& message, int x, int y); // Add overload
};
#endif
