#pragma once

#include <memory>
#include <SDL.h>
#include "SmartPointers.h"
#include "asset_manager/AssetManager.h"
#include "ecs/ECS.h"
#include "event_bus/EventBus.h"


constexpr auto FPS = 1000;
constexpr auto MILLIS_PER_FRAME = 1000 / FPS;

class Game {
    Uint32 lastFrameTicks{0};
    bool isRunning{false};
    bool isDebugMode{false};
    SDL_Rect camera{};
    Unique<SDL_Window> window;
    Unique<SDL_Renderer> renderer;
    Unique<Registry> registry{new Registry()};
    Unique<AssetManager> assetManager{new AssetManager()};
    Unique<EventBus> eventBus{new EventBus()};

public:
    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;

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
