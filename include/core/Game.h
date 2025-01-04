#pragma once

#include <memory>
#include <SDL_render.h>
#include <SDL_video.h>
#include "Core.h"


class Game {
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
