#pragma once

#include <memory>
#include <SDL_render.h>
#include <SDL_video.h>

#include "Core.h"

struct SDLWindowDeleter {
    void operator()(SDL_Window* window) const {
        SDL_DestroyWindow(window);
    }
};

struct SDLRendererDeleter {
    void operator()(SDL_Renderer* renderer) const {
        SDL_DestroyRenderer(renderer);
    }
};

class Game {
    bool isRunning{false};
    UniqueWithDeleter<SDL_Window, SDLWindowDeleter> window;
    UniqueWithDeleter<SDL_Renderer, SDLRendererDeleter> renderer;

public:
    Game();

    void Initialize();
    void Run();

    void ProcessInput();
    void Update();
    void Render();

    void Destroy();
};
