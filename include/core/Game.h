#pragma once

#include <memory>
#include <SDL.h>
#include "Core.h"


constexpr auto FPS = 60;
constexpr auto MILLIS_PER_FRAME = 1000 / FPS;

class Game {
    Uint32 lastFrameTicks{0};
    bool isRunning{false};
    Unique<SDL_Window> window;
    Unique<SDL_Renderer> renderer;

public:
    int windowWidth{0};
    int windowHeight{0};

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
