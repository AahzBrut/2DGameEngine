#include "core/Game.h"

#include <iostream>
#include <ostream>
#include <SDL.h>

#include "core/Core.h"

Game::Game() {
    std::cout << "Game constructor called." << std::endl;
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return;
    }

    window = UniqueWithDeleter<SDL_Window, SDLWindowDeleter>(SDL_CreateWindow(
        nullptr,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_BORDERLESS
    ));
    if (!window) {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = UniqueWithDeleter<SDL_Renderer, SDLRendererDeleter>(SDL_CreateRenderer(window.get(), -1, 0));
    if (!renderer) {
        std::cerr << "Error creating SDL renderer: " << SDL_GetError() << std::endl;
        return;
    }

    isRunning = true;
}

void Game::Run() {
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                std::cout << "Quitting." << std::endl;
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
            default:
                break;
        }
    }
}

void Game::Update() {}
void Game::Render() {}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer.get());
    SDL_DestroyWindow(window.get());
    SDL_Quit();
}
