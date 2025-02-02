#pragma once

#include <memory>
#include <SDL.h>
#include <sol/state.hpp>

#include "SmartPointers.h"
#include "asset_manager/AssetManager.h"
#include "audio_manager/AudioManager.h"
#include "ecs/ECS.h"
#include "event_bus/EventBus.h"


constexpr auto FPS = 1000;
constexpr auto MILLIS_PER_FRAME = 1000 / FPS;

class Game {
    Uint32 lastFrameTicks{0};
    bool isRunning{false};
    bool isDebugMode{false};
    SDL_Rect camera{};

    sol::state lua;

    Unique<SDL_Window> window;
    Unique<SDL_Renderer> renderer;
    Unique<Registry> registry{new Registry()};
    Unique<AssetManager> assetManager{new AssetManager()};
    Unique<EventBus> eventBus{new EventBus()};
    Unique<AudioManager> audioManager{new AudioManager(assetManager)};

public:
    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;

    Game();
    ~Game();

    void Initialize();
    void Setup();
    void Run();

    void ProcessInput();
    void Update();
    void Render() const;

    void Destroy();
};
