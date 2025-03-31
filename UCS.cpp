// Version: 0.0.4
// Credits:
// @skyliuswastaken on Discord: Main Developer
// @_na69_ on Discord: Engine Contributor
// @traox_ on Discord: Engine Contributor & Main Designer

#include "UCS.h"

#include <iostream>

#include <algorithm>

VEGA16::VEGA16() :
    running(false),
    window(nullptr),
    renderer(nullptr),
    catTexture(nullptr),
    gunTexture(nullptr),
    font(nullptr),
    frameCount(0),
    fps(0),
    lastTime(0),
    isCrushed(false),
    isJumping(false),
    jumpCount(0),
    verticalVelocity(0),
    debugMode(false),
    isFacingRight(true),
    currentDebugRenderer(DebugRenderer::NONE) {
    player = {
      SCREEN_WIDTH / 2 - BLOCK_SIZE / 2,
      SCREEN_HEIGHT - BLOCK_SIZE - 50,
      BLOCK_SIZE,
      BLOCK_SIZE
    };
    prevPlayerX = player.x;
    prevPlayerY = player.y;
}

VEGA16::~VEGA16() {
    clean();
}

bool VEGA16::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Untitled Cat Shooter (VEGA 16 Engine)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    catTexture = IMG_LoadTexture(renderer, "cat.png");
    if (!catTexture) {
        std::cerr << "Failed to load cat texture! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    SDL_QueryTexture(catTexture, nullptr, nullptr, &catWidth, &catHeight);

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("font.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    gunTexture = IMG_LoadTexture(renderer, "gun.png");
    if (!gunTexture) {
        std::cerr << "Failed to load gun texture! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    trees.clear();
    trees.push_back({
      100,
      SCREEN_HEIGHT - 50,
      0
        });
    trees.push_back({
      SCREEN_WIDTH / 2 - 100,
      SCREEN_HEIGHT - 50,
      0
        });
    trees.push_back({
      SCREEN_WIDTH - 200,
      SCREEN_HEIGHT - 50,
      0
        });

    updateTrees();

    running = true;
    isCrushed = false;
    bullets.clear();
    lastTime = SDL_GetTicks();

    return true;
}

void VEGA16::run() {
    while (running) {
        frameStart = SDL_GetTicks();

        handleEvents();
        if (!isCrushed) {
            update();
        }
        render();

        capFrameRate();
    }
}

void VEGA16::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (!isCrushed) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_w:
                    if (!isJumping) {
                        verticalVelocity = -JUMP_SPEED;
                        isJumping = true;
                        jumpCount = 1;
                    }
                    else if (jumpCount < MAX_JUMP_COUNT) {
                        verticalVelocity = -JUMP_SPEED * 1.5;
                        jumpCount = 2;
                    }
                    break;
                    updateInputHistory(event.key.keysym.sym);

                case SDLK_a:
                    if (player.x > 0) {
                        player.x -= MOVE_SPEED;
                        isFacingRight = false;
                    }
                    updateInputHistory(event.key.keysym.sym);
                    break;

                case SDLK_d:
                    if (player.x < SCREEN_WIDTH - BLOCK_SIZE) {
                        player.x += MOVE_SPEED;
                        isFacingRight = true;
                    }
                    updateInputHistory(event.key.keysym.sym);
                    break;

                case SDLK_f:
                    fireBullet();
                    break;
                    updateInputHistory(event.key.keysym.sym);

                case SDLK_s:
                    if (player.y + BLOCK_SIZE >= SCREEN_HEIGHT - 90) {
                        isCrushed = true;
                    }
                    else {
                        player.y += MOVE_SPEED;
                    }
                    break;
                    updateInputHistory(event.key.keysym.sym);

                case SDLK_RETURN:
                    if (isCrushed) {
                        restartGame();
                    }
                    break;

                case SDLK_F4:
                    toggleDebugMode();
                    break;

                case SDLK_LEFT:
                    if (debugMode) {
                        cycleDebugRenderer(false);
                    }
                    break;

                case SDLK_RIGHT:
                    if (debugMode) {
                        cycleDebugRenderer(true);
                    }
                    break;
                }
            }
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            restartGame();
        }
    }
}

void VEGA16::update() {
    if (isJumping) {
        verticalVelocity += GRAVITY;
        player.y += verticalVelocity;

        if (verticalVelocity >= 0 && player.y >= SCREEN_HEIGHT - BLOCK_SIZE - 50) {
            player.y = SCREEN_HEIGHT - BLOCK_SIZE - 50;
            isJumping = false;
            jumpCount = 0;
            verticalVelocity = 0;
        }
    }
    else {
        if (player.y < SCREEN_HEIGHT - BLOCK_SIZE - 50) {
            verticalVelocity += GRAVITY;
            player.y += verticalVelocity;
        }
    }

    updateBullets();

    checkCollisions();

    updateTrees();

    prevPlayerX = player.x;
    prevPlayerY = player.y;
}

void VEGA16::render() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 65, 153, 10, 255);
    SDL_Rect groundRect = {
      0,
      SCREEN_HEIGHT - 50,
      SCREEN_WIDTH,
      50
    };
    SDL_RenderFillRect(renderer, &groundRect);

    for (const auto& tree : trees) {
        drawTree(tree);
    }

    const int spriteHeight = BLOCK_SIZE;
    const int spriteWidth = (spriteHeight * catWidth) / catHeight;
    SDL_Rect playerRect = {
      player.x,
      player.y,
      spriteWidth,
      spriteHeight
    };
    SDL_RendererFlip catFlip = isFacingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, catTexture, nullptr, &playerRect, 0, nullptr, catFlip);

    int gunX = isFacingRight ? (player.x + spriteWidth - 15) : (player.x - 15);
    int gunY = player.y + (spriteHeight / 2) - 5;
    SDL_Rect gunRect = {
      gunX,
      gunY,
      30,
      15
    };
    SDL_RendererFlip gunFlip = isFacingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, gunTexture, nullptr, &gunRect, 0, nullptr, gunFlip);

    for (const auto& bullet : bullets) {
        if (bullet.active) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_Rect bodyRect = {
              bullet.rect.x,
              bullet.rect.y + 2,
              bullet.rect.w,
              bullet.rect.h - 4
            };
            SDL_RenderFillRect(renderer, &bodyRect);

            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
            SDL_Rect tipRect1 = {
              bullet.rect.x,
              bullet.rect.y,
              bullet.rect.w,
              2
            };
            SDL_RenderFillRect(renderer, &tipRect1);
            SDL_Rect tipRect2 = {
              bullet.rect.x,
              bullet.rect.y + bullet.rect.h - 2,
              bullet.rect.w,
              2
            };
            SDL_RenderFillRect(renderer, &tipRect2);
        }
    }

    if (isCrushed) {
        displayMessage("YOU WERE CRUSHED!", SCREEN_HEIGHT / 2 - 40);
        displayMessage("Press ENTER to Restart", SCREEN_HEIGHT / 2 + 40);
    }

    if (debugMode) {
        renderDebugInfo();
    }

    SDL_RenderPresent(renderer);
}

void VEGA16::toggleDebugMode() {
    debugMode = !debugMode;
    if (debugMode) {
        currentDebugRenderer = DebugRenderer::COLLISION_BOXES;
    }
    else {
        currentDebugRenderer = DebugRenderer::NONE;
    }
}

void VEGA16::cycleDebugRenderer(bool forward) {
    int rendererCount = static_cast <int> (DebugRenderer::COUNT);
    int currentIndex = static_cast <int> (currentDebugRenderer);

    if (forward) {
        currentIndex = (currentIndex + 1) % rendererCount;
        if (currentIndex == 0) currentIndex = 1;
    }
    else {
        currentIndex = (currentIndex - 1 + rendererCount) % rendererCount;
        if (currentIndex == 0) currentIndex = rendererCount - 1;
    }

    currentDebugRenderer = static_cast <DebugRenderer> (currentIndex);
}

std::string VEGA16::getDebugRendererName(DebugRenderer renderer) {
    switch (renderer) {
    case DebugRenderer::NONE:
        return "None";
    case DebugRenderer::COLLISION_BOXES:
        return "Collision Boxes";
    case DebugRenderer::PHYSICS:
        return "Physics";
    case DebugRenderer::BULLET_PATHS:
        return "Bullet Paths";
    case DebugRenderer::PERFORMANCE:
        return "Performance";
    case DebugRenderer::PLAYER_STATS:
        return "Player Stats";
    case DebugRenderer::GAME_STATE:
        return "Game State";
    case DebugRenderer::INPUT_DEBUG:
        return "Input Debug";
    case DebugRenderer::COLLISION_DEBUG:
        return "Collision Debug";
    default:
        return "Unknown";
    }
}

void VEGA16::renderDebugInfo() {
    std::string rendererName = "Debug: " + getDebugRendererName(currentDebugRenderer);
    displayMessage(rendererName, 30);

    std::string fpsText = "FPS: " + std::to_string(fps);
    displayMessage(fpsText, 60);

    switch (currentDebugRenderer) {
    case DebugRenderer::COLLISION_BOXES:
        renderCollisionBoxes();
        break;
    case DebugRenderer::PHYSICS:
        renderPhysicsInfo();
        break;
    case DebugRenderer::BULLET_PATHS:
        renderBulletPaths();
        break;
    case DebugRenderer::PERFORMANCE:
        renderPerformanceInfo();
        break;
    case DebugRenderer::PLAYER_STATS:
        renderPlayerStats();
        break;
    case DebugRenderer::GAME_STATE:
        renderGameState();
        break;
    case DebugRenderer::INPUT_DEBUG:
        renderInputDebug();
        break;
    case DebugRenderer::COLLISION_DEBUG:
        renderCollisionDebug();
        break;
    default:
        break;
    }
}

void VEGA16::displayMessage(const std::string& message, int yOffset) {
    SDL_Color white = {
      255,
      255,
      255
    };
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), white);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_Rect textRect = {
      (SCREEN_WIDTH - textWidth) / 2,
      yOffset,
      textWidth,
      textHeight
    };
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void VEGA16::displayMessage(const std::string& message, int x, int y) {
    SDL_Color white = {
      255,
      255,
      255
    };
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), white);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_Rect textRect = {
      x,
      y,
      textWidth,
      textHeight
    };
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void VEGA16::renderPlayerStats() {
    std::string position = "Player Position: (" + std::to_string(player.x) + ", " + std::to_string(player.y) + ")";
    displayMessage(position, 90);

    std::string jumpStatus = "Jumping: " + std::string(isJumping ? "Yes" : "No");
    displayMessage(jumpStatus, 120);
}

void VEGA16::renderGameState() {
    std::string gameRunning = running ? "Game State: Running" : "Game State: Paused";
    displayMessage(gameRunning, 90);

    std::string crushedStatus = isCrushed ? "Player is Crushed!" : "Player is Alive";
    displayMessage(crushedStatus, 120);

    std::string activeObjects = "Active Bullets: " + std::to_string(bullets.size()) +
        ", Trees: " + std::to_string(trees.size());
    displayMessage(activeObjects, 150);
}

void VEGA16::renderCollisionBoxes() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
    SDL_RenderDrawRect(renderer, &player);

    for (const auto& tree : trees) {
        if (tree.damage < 100) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128);
            SDL_RenderDrawRect(renderer, &tree.trunk);

            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 128);
            for (const auto& leaf : tree.leaves) {
                SDL_RenderDrawRect(renderer, &leaf);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 128);
    for (const auto& bullet : bullets) {
        if (bullet.active) {
            SDL_RenderDrawRect(renderer, &bullet.rect);
        }
    }

    std::string collisionInfo = "Player x,y: " + std::to_string(player.x) + "," + std::to_string(player.y);
    displayMessage(collisionInfo, 90);

    std::string bulletCount = "Active bullets: " + std::to_string(bullets.size());
    displayMessage(bulletCount, 120);
}

void VEGA16::updateInputHistory(SDL_Keycode key) {
    recentInputs.push_back(key);
    if (recentInputs.size() > INPUT_HISTORY_SIZE) {
        recentInputs.erase(recentInputs.begin());
    }
}

void VEGA16::renderInputDebug() {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    std::string inputState = "Input State: ";
    inputState += keystate[SDL_SCANCODE_W] ? "W " : "";
    inputState += keystate[SDL_SCANCODE_A] ? "A " : "";
    inputState += keystate[SDL_SCANCODE_S] ? "S " : "";
    inputState += keystate[SDL_SCANCODE_D] ? "D " : "";
    inputState += keystate[SDL_SCANCODE_F] ? "F " : "";
    displayMessage(inputState, 90);

    std::string inputHistory = "Input History: ";
    for (const auto& key : recentInputs) {
        switch (key) {
        case SDLK_w:
            inputHistory += "W ";
            break;
        case SDLK_a:
            inputHistory += "A ";
            break;
        case SDLK_s:
            inputHistory += "S ";
            break;
        case SDLK_d:
            inputHistory += "D ";
            break;
        case SDLK_f:
            inputHistory += "F ";
            break;
        default:
            inputHistory += "? ";
            break;
        }
    }
    displayMessage(inputHistory, 120);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    SDL_Rect inputBox = {
      SCREEN_WIDTH - 150,
      30,
      120,
      120
    };
    SDL_RenderDrawRect(renderer, &inputBox);

    SDL_SetRenderDrawColor(renderer,
        keystate[SDL_SCANCODE_W] ? 0 : 255,
        keystate[SDL_SCANCODE_W] ? 255 : 0,
        0, 255);
    SDL_Rect wKey = {
      SCREEN_WIDTH - 100,
      50,
      40,
      20
    };
    SDL_RenderFillRect(renderer, &wKey);
    displayMessage("W", 55, SCREEN_WIDTH - 90);

    SDL_SetRenderDrawColor(renderer,
        keystate[SDL_SCANCODE_A] ? 0 : 255,
        keystate[SDL_SCANCODE_A] ? 255 : 0,
        0, 255);
    SDL_Rect aKey = {
      SCREEN_WIDTH - 120,
      80,
      40,
      20
    };
    SDL_RenderFillRect(renderer, &aKey);
    displayMessage("A", 85, SCREEN_WIDTH - 110);

    SDL_SetRenderDrawColor(renderer,
        keystate[SDL_SCANCODE_S] ? 0 : 255,
        keystate[SDL_SCANCODE_S] ? 255 : 0,
        0, 255);
    SDL_Rect sKey = {
      SCREEN_WIDTH - 100,
      80,
      40,
      20
    };
    SDL_RenderFillRect(renderer, &sKey);
    displayMessage("S", 85, SCREEN_WIDTH - 90);

    SDL_SetRenderDrawColor(renderer,
        keystate[SDL_SCANCODE_D] ? 0 : 255,
        keystate[SDL_SCANCODE_D] ? 255 : 0,
        0, 255);
    SDL_Rect dKey = {
      SCREEN_WIDTH - 80,
      80,
      40,
      20
    };
    SDL_RenderFillRect(renderer, &dKey);
    displayMessage("D", 85, SCREEN_WIDTH - 70);

    SDL_SetRenderDrawColor(renderer,
        keystate[SDL_SCANCODE_F] ? 0 : 255,
        keystate[SDL_SCANCODE_F] ? 255 : 0,
        0, 255);
    SDL_Rect fKey = {
      SCREEN_WIDTH - 100,
      110,
      40,
      20
    };
    SDL_RenderFillRect(renderer, &fKey);
    displayMessage("FIRE", 115, SCREEN_WIDTH - 90);
}

void VEGA16::renderCollisionDebug() {
    SDL_Point testPoints[] = {
      {
        player.x, player.y
      }, // Top Left
      {
        player.x + player.w / 2,
        player.y
      }, // Top Centre
      {
        player.x + player.w,
        player.y
      }, // Top Right
      {
        player.x,
        player.y + player.h / 2
      }, // Mid Left
      {
        player.x + player.w,
        player.y + player.h / 2
      }, // Mid Right
      {
        player.x,
        player.y + player.h
      }, // Bottom Left
      {
        player.x + player.w / 2,
        player.y + player.h
      }, // Bottom Centre
      {
        player.x + player.w,
        player.y + player.h
      } // Bottom Right
    };

    for (const auto& point : testPoints) {
        bool colliding = false;

        for (const auto& tree : trees) {
            if (tree.damage < 100) {
                if (SDL_PointInRect(&point, &tree.trunk)) {
                    colliding = true;
                    lastCollisionPoint = point;
                    break;
                }

                for (const auto& leaf : tree.leaves) {
                    if (SDL_PointInRect(&point, &leaf)) {
                        colliding = true;
                        lastCollisionPoint = point;
                        break;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,
            colliding ? 255 : 0,
            colliding ? 0 : 255,
            0, 255);
        SDL_Rect pointRect = {
          point.x - 2,
          point.y - 2,
          5,
          5
        };
        SDL_RenderFillRect(renderer, &pointRect);
    }

    if (lastCollisionPoint.x != 0 || lastCollisionPoint.y != 0) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect lastPoint = {
          lastCollisionPoint.x - 3,
          lastCollisionPoint.y - 3,
          7,
          7
        };
        SDL_RenderFillRect(renderer, &lastPoint);
    }

    std::string collisionInfo = "Collision Points: 8 (Red=Colliding)";
    displayMessage(collisionInfo, 90);

    std::string lastCollision = "Last Collision: (" +
        std::to_string(lastCollisionPoint.x) + "," +
        std::to_string(lastCollisionPoint.y) + ")";
    displayMessage(lastCollision, 120);
}

void VEGA16::renderPhysicsInfo() {
    std::string jumpInfo = "Jumping: " + std::string(isJumping ? "YES" : "NO") +
        " | Jump count: " + std::to_string(jumpCount);
    displayMessage(jumpInfo, 90);

    std::string velocityInfo = "Velocity: " + std::to_string(verticalVelocity);
    displayMessage(velocityInfo, 120);
    int arrowLength = verticalVelocity * 3;

    int centerX = player.x + player.w / 2;
    int centerY = player.y + player.h / 2;

    thickLineRGBA(renderer,
        centerX, centerY,
        centerX, centerY + arrowLength,
        3, 255, 0, 255, 255);

    if (arrowLength != 0) {
        int arrowSize = 10;
        int arrowDir = (arrowLength > 0) ? 1 : -1;

        filledTrigonRGBA(renderer,
            centerX, centerY + arrowLength,
            centerX - arrowSize, centerY + arrowLength - arrowSize * arrowDir,
            centerX + arrowSize, centerY + arrowLength - arrowSize * arrowDir,
            255, 0, 255, 255);
    }
}

void VEGA16::renderBulletPaths() {
    for (const auto& bullet : bullets) {
        if (bullet.active) {
            int pathX = bullet.rect.x;
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 128);

            for (int i = 0; i < 30; i++) {
                pathX += BULLET_SPEED;

                if (i % 2 == 0 && pathX < SCREEN_WIDTH) {
                    SDL_Rect pathPoint = {
                      pathX,
                      bullet.rect.y,
                      3,
                      3
                    };
                    SDL_RenderFillRect(renderer, &pathPoint);
                }
            }

            std::string bulletInfo = "Bullet pos: " + std::to_string(bullet.rect.x) + "," + std::to_string(bullet.rect.y);
            displayMessage(bulletInfo, 90);
        }
    }

    std::string bulletCount = "Active bullets: " + std::to_string(bullets.size());
    displayMessage(bulletCount, 120);
}

void VEGA16::renderPerformanceInfo() {
    Uint32 currentTime = SDL_GetTicks();

    Uint32 frameTime = SDL_GetTicks() - frameStart;

    std::string fpsInfo = "FPS: " + std::to_string(fps) + " / " + std::to_string(FPS_CAP);
    displayMessage(fpsInfo, 90);

    std::string frameTimeInfo = "Frame time: " + std::to_string(frameTime) + "ms";
    displayMessage(frameTimeInfo, 120);

    std::string memoryInfo = "Active game objects: " +
        std::to_string(bullets.size()) + " bullets, " +
        std::to_string(trees.size()) + " trees";
    displayMessage(memoryInfo, 150);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    SDL_Rect graphBg = {
      SCREEN_WIDTH - 210,
      SCREEN_HEIGHT - 120,
      200,
      100
    };
    SDL_RenderDrawRect(renderer, &graphBg);

    int graphY = SCREEN_HEIGHT - 120 + 100 - (fps * 100 / FPS_CAP);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect fpsMarker = {
      SCREEN_WIDTH - 200,
      graphY,
      180,
      2
    };
    SDL_RenderFillRect(renderer, &fpsMarker);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
    SDL_Rect capLine = {
      SCREEN_WIDTH - 200,
      SCREEN_HEIGHT - 120,
      180,
      1
    };
    SDL_RenderFillRect(renderer, &capLine);

    std::string graphLabel = "FPS Graph";
    displayMessage(graphLabel, SCREEN_HEIGHT - 130);
}

void VEGA16::drawTree(const Tree& tree) {
    if (tree.damage >= 100) {
        return;
    }

    if (tree.damage >= 66) {
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderFillRect(renderer, &tree.trunk);
    }
    else if (tree.damage >= 33) {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_RenderFillRect(renderer, &tree.trunk);

        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_Rect damagedTrunkRect = {
          tree.trunk.x,
          tree.trunk.y,
          tree.trunk.w / 2,
          tree.trunk.h / 2
        };
        SDL_RenderFillRect(renderer, &damagedTrunkRect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_RenderFillRect(renderer, &tree.trunk);
    }

    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
    for (const auto& leaf : tree.leaves) {
        SDL_RenderFillRect(renderer, &leaf);
    }
}

void VEGA16::updateTrees() {
    for (auto& tree : trees) {
        tree.trunk = {
          tree.x - 50,
          tree.y - 250,
          100,
          250
        };
        tree.leaves.clear();

        if (tree.damage < 33) {
            tree.leaves.push_back({
              tree.x - 120,
              tree.y - 300,
              240,
              60
                }); // Bottom
            tree.leaves.push_back({
              tree.x - 90,
              tree.y - 350,
              180,
              60
                }); // Middle
            tree.leaves.push_back({
              tree.x - 60,
              tree.y - 400,
              120,
              60
                }); // Top
        }
        else if (tree.damage < 66) {
            tree.leaves.push_back({
              tree.x - 120,
              tree.y - 300,
              240,
              60
                }); // Bottom
            tree.leaves.push_back({
              tree.x - 90,
              tree.y - 350,
              180,
              60
                }); // Middle
        }
        else if (tree.damage < 100) {
            tree.leaves.push_back({
              tree.x - 120,
              tree.y - 300,
              240,
              60
                }); // Bottom only
        }
    }
}

void VEGA16::fireBullet() {
    Bullet newBullet;
    int spriteHeight = BLOCK_SIZE;
    int spriteWidth = (spriteHeight * catWidth) / catHeight;

    if (isFacingRight) {
        newBullet.rect = {
          player.x + spriteWidth - 10,
          player.y + (spriteHeight / 2) - 5,
          10,
          5
        };
        newBullet.direction = 1;
    }
    else {
        newBullet.rect = {
          player.x - 10,
          player.y + (spriteHeight / 2) - 5,
          10,
          5
        };
        newBullet.direction = -1;
    }
    newBullet.active = true;
    bullets.push_back(newBullet);
}

void VEGA16::updateBullets() {
    for (auto& bullet : bullets) {
        if (bullet.active) {
            bullet.rect.x += bullet.direction * BULLET_SPEED;
            if (bullet.rect.x > SCREEN_WIDTH || bullet.rect.x + bullet.rect.w < 0) {
                bullet.active = false;
            }
        }
    }

    bullets.erase(
        std::remove_if(
            bullets.begin(),
            bullets.end(),
            [](const Bullet& b) {
                return !b.active;
            }
        ),
        bullets.end()
    );
}

void VEGA16::checkCollisions() {
    for (auto it = bullets.begin(); it != bullets.end();) {
        if (!it->active) {
            ++it;
            continue;
        }

        bool bulletHit = false;
        for (auto& tree : trees) {
            if (tree.damage < 100 && SDL_HasIntersection(&it->rect, &tree.trunk)) {
                tree.damage += 33;
                if (tree.damage > 100) {
                    tree.damage = 100;
                }
                bulletHit = true;
                break;
            }
        }

        if (bulletHit) {
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }

    for (const auto& tree : trees) {
        if (tree.damage < 100 && SDL_HasIntersection(&player, &tree.trunk)) {
            isCrushed = true;
            return;
        }
    }

    bool isOnLeaves = false;
    for (const auto& tree : trees) {
        for (const auto& leaf : tree.leaves) {
            if (SDL_HasIntersection(&player, &leaf)) {
                if (player.y + player.h <= leaf.y + 10 && verticalVelocity >= 0) {
                    player.y = leaf.y - player.h;
                    isJumping = false;
                    verticalVelocity = 0;
                    isOnLeaves = true;
                }
            }
        }
    }

    if (!isOnLeaves && !isJumping && player.y < SCREEN_HEIGHT - BLOCK_SIZE - 50) {
        isJumping = true;
    }
}

void VEGA16::restartGame() {
    player.x = SCREEN_WIDTH / 2 - BLOCK_SIZE / 2;
    player.y = SCREEN_HEIGHT - BLOCK_SIZE - 50;
    isJumping = false;
    jumpCount = 0;
    verticalVelocity = 0;
    isCrushed = false;
    bullets.clear();

    for (auto& tree : trees) {
        tree.damage = 0;
    }
    updateTrees();
}

void VEGA16::capFrameRate() {
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }

    frameCount++;
    if (SDL_GetTicks() - lastTime >= 1000) {
        fps = frameCount;
        frameCount = 0;
        lastTime = SDL_GetTicks();
    }
}

void VEGA16::clean() {
    if (catTexture) {
        SDL_DestroyTexture(catTexture);
        catTexture = nullptr;
    }

    if (gunTexture) {
        SDL_DestroyTexture(gunTexture);
        gunTexture = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}