#pragma once

#include <memory>
#include <SDL.h>
#include "SmartPointers.h"
#include "asset_manager/AssetManager.h"
#include "ecs/ECS.h"


constexpr auto FPS = 1000;
constexpr auto MILLIS_PER_FRAME = 1000 / FPS;

class Game {
    Uint32 lastFrameTicks{0};
    bool isRunning{false};
    Unique<SDL_Window> window;
    Unique<SDL_Renderer> renderer;
    Unique<Registry> registry{new Registry()};
    Unique<AssetManager> assetManager{new AssetManager()};

public:
    int windowWidth{0};
    int windowHeight{0};

    Game();
    ~Game();

    void Initialize();
    void LoadLevel(int level);
    void Setup();
    void Run();

    void ProcessInput();
    void Update();
    void Render() const;

    void Destroy();
};
