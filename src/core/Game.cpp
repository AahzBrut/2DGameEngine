#include "core/Game.h"

#include <format>
#include <iostream>
#include <ostream>
#include <SDL.h>
#include <SDL_image.h>

#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "core/Logger.h"
#include "glm/vec2.hpp"
#include "systems/MovementSystem.h"


Game::Game() {
    LOG("Game constructor called.");
}

Game::~Game() {
    LOG("Game destructor called.");
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

    window = Unique<SDL_Window>(SDL_CreateWindow(
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

    renderer = Unique<SDL_Renderer>(SDL_CreateRenderer(window.get(), -1, 0));
    if (!renderer) {
        std::cerr << "Error creating SDL renderer: " << SDL_GetError() << std::endl;
        return;
    }

    //SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN);
    SDL_GL_SetSwapInterval(0);

    isRunning = true;
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup() const {
    registry->AddSystem<MovementSystem>();

    playerPosition = {10, 20};
    playerVelocity = {60, 0};

    auto tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2{0, 0}, glm::vec2{1, 1}, 0.0);
    tank.AddComponent<VelocityComponent>(glm::vec2{10, 10});

     auto truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2{0, 0}, glm::vec2{1, 1}, 0.0);
    truck.AddComponent<VelocityComponent>(glm::vec2{5, 0});
}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
        registry->Update();
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

void Game::Update() {
    if (const auto timeToWait = MILLIS_PER_FRAME - (SDL_GetTicks() - lastFrameTicks);
        timeToWait <= MILLIS_PER_FRAME && timeToWait > 0) {
        SDL_Delay(MILLIS_PER_FRAME - (SDL_GetTicks() - lastFrameTicks));
    }
    const auto deltaTime = static_cast<float>(SDL_GetTicks() - lastFrameTicks) / 1000.0f;
    playerPosition += playerVelocity * deltaTime;

    registry->GetSystem<MovementSystem>().Update(deltaTime);

    lastFrameTicks = SDL_GetTicks();
}

void Game::Render() const {
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 64, 255);
    SDL_RenderClear(renderer.get());
    // Render all game objects and UI

    const auto surface = IMG_Load("./assets/images/tank-tiger-right.png");
    const auto texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    SDL_FreeSurface(surface);
    const SDL_Rect dstRect{static_cast<int>(playerPosition.x), static_cast<int>(playerPosition.y), 64, 64};
    SDL_RenderCopy(renderer.get(), texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer.get());
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer.release());
    SDL_DestroyWindow(window.release());
    SDL_Quit();
}
