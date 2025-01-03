#include "core/Game.h"

#include <iostream>
#include <ostream>
#include <SDL.h>
#include <SDL_image.h>


Game::Game() {
    std::cout << "Game constructor called." << std::endl;
}

Game::~Game() {
    std::cout << "Game destructor called." << std::endl;
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = 800;
    windowHeight = 600;

    window = UniqueWithDeleter<SDL_Window, SDLWindowDeleter>(SDL_CreateWindow(
        nullptr,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
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

    SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN);

    isRunning = true;
}

void Game::Setup() {}

void Game::Run() {
    Setup();
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

void Game::Render() const {
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 64, 255);
    SDL_RenderClear(renderer.get());
    // Render all game objects and UI

    const auto surface = IMG_Load("./assets/images/tank-tiger-right.png");
    const auto texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    SDL_FreeSurface(surface);
    constexpr SDL_Rect dstRect{200, 200, 64, 64};
    SDL_RenderCopy(renderer.get(), texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer.get());
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer.get());
    SDL_DestroyWindow(window.get());
    SDL_Quit();
}
