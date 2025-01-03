#pragma once

#include <memory>

#include "Core.h"


class Game {
    bool isRunning{false};
    UniqueWithDeleter<SDL_Window, SDLWindowDeleter> window;
    UniqueWithDeleter<SDL_Renderer, SDLRendererDeleter> renderer;

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
