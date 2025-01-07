#include "core/Game.h"

#include <format>
#include <fstream>
#include <iostream>
#include <ostream>
#include <SDL2/SDL.h>

#include "components/AnimationComponent.h"
#include "components/BoxColliderComponent.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "core/Logger.h"
#include "glm/vec2.hpp"
#include "systems/AnimationSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/MovementSystem.h"
#include "systems/RenderColliderSystem.h"
#include "systems/RenderSystem.h"


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

// ReSharper disable once CppMemberFunctionMayBeConst
void Game::LoadLevel([[maybe_unused]] int level) {
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();

    assetManager->LoadTexture(renderer, "tank", "./assets/images/tank-panther-right.png");
    assetManager->LoadTexture(renderer, "truck", "./assets/images/truck-ford-right.png");
    assetManager->LoadTexture(renderer, "jungle-map", "./assets/tilemaps/jungle.png");
    assetManager->LoadTexture(renderer, "chopper", "./assets/images/chopper.png");
    assetManager->LoadTexture(renderer, "radar", "./assets/images/radar.png");

    const auto &jungleMapSprite = assetManager->GetTexture("jungle-map");
    constexpr auto mapHeight = 20;
    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");
    for (auto y = 0; y < mapHeight; y++) {
        constexpr auto mapWidth = 25;
        for (auto x = 0; x < mapWidth; x++) {
            constexpr auto tileScale = 1.0;
            constexpr auto tileSize = 32;
            char character;
            mapFile.get(character);
            const auto srcRectY = tileSize * std::atoi(&character); // NOLINT(*-err34-c)
            mapFile.get(character);
            const auto srcRectX = tileSize * std::atoi(&character); // NOLINT(*-err34-c)
            mapFile.ignore();
            registry->CreateEntity()
                    .AddComponent<TransformComponent>(glm::vec2{x * tileScale * tileSize, y * tileScale * tileSize},
                                                      glm::vec2{tileScale, tileScale}, 0.0)
                    .AddComponent<SpriteComponent>(jungleMapSprite, SDL_Rect{srcRectX, srcRectY, tileSize, tileSize},
                                                   0);
        }
    }

    mapFile.close();

    const auto &tankSprite = assetManager->GetTexture("tank");
    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{0, 0}, glm::vec2{1, 1}, 0.0)
            .AddComponent<VelocityComponent>(glm::vec2{10, 10})
            .AddComponent<SpriteComponent>(tankSprite, SDL_Rect{0, 0, tankSprite.width, tankSprite.height},
                                           2, SDL_Color{255, 0, 0, 255})
            .AddComponent<BoxColliderComponent>(32, 32);

    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{200, 200}, glm::vec2{1, 1}, 0.0)
            .AddComponent<VelocityComponent>(glm::vec2{5, -10})
            .AddComponent<SpriteComponent>(tankSprite, SDL_Rect{0, 0, tankSprite.width, tankSprite.height},
                                           2, SDL_Color{0, 255, 0, 255})
            .AddComponent<BoxColliderComponent>(32, 32);

    const auto &truckSprite = assetManager->GetTexture("truck");
    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{0, 0}, glm::vec2{1, 1}, 0.0)
            .AddComponent<VelocityComponent>(glm::vec2{5, 0})
            .AddComponent<SpriteComponent>(truckSprite, SDL_Rect{0, 0, truckSprite.width, truckSprite.height}, 1)
            .AddComponent<BoxColliderComponent>(32, 32);

    const auto &chopperSprite = assetManager->GetTexture("chopper");
    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{100, 100}, glm::vec2{2, 2}, 0.0)
            .AddComponent<VelocityComponent>(glm::vec2{0, -10})
            .AddComponent<SpriteComponent>(chopperSprite, SDL_Rect{0, 0, chopperSprite.width, chopperSprite.height}, 3)
            .AddComponent<BoxColliderComponent>(32, 32)
            .AddComponent<AnimationComponent>(List{
                                                  SDL_Rect{0, 0, 32, 32},
                                                  SDL_Rect{32, 0, 32, 32},
                                              }, 0, 8.f, true, 0);

    const auto &radarSprite = assetManager->GetTexture("radar");
    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{windowWidth - 72, 8}, glm::vec2{1, 1}, 0.0)
            .AddComponent<SpriteComponent>(radarSprite, SDL_Rect{0, 0, radarSprite.width, radarSprite.height}, 3)
            .AddComponent<AnimationComponent>(List{
                                                  SDL_Rect{0, 0, 64, 64},
                                                  SDL_Rect{64, 0, 64, 64},
                                                  SDL_Rect{128, 0, 64, 64},
                                                  SDL_Rect{192, 0, 64, 64},
                                                  SDL_Rect{256, 0, 64, 64},
                                                  SDL_Rect{320, 0, 64, 64},
                                                  SDL_Rect{384, 0, 64, 64},
                                                  SDL_Rect{448, 0, 64, 64},
                                              }, 0, 8.f, true, 0);
}

void Game::Setup() {
    LoadLevel(1);
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
                if (event.key.keysym.sym == SDLK_d) {
                    isDebugMode = !isDebugMode;
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

    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update(deltaTime);
    registry->GetSystem<CollisionSystem>().Update();

    lastFrameTicks = SDL_GetTicks();
}

void Game::Render() const {
    SDL_SetRenderDrawColor(renderer.get(), 63, 63, 63, 255);
    SDL_RenderClear(renderer.get());
    // Render all game objects and UI

    registry->GetSystem<RenderSystem>().Render(renderer);
    if (isDebugMode) registry->GetSystem<RenderColliderSystem>().Render(renderer);

    SDL_RenderPresent(renderer.get());
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer.release());
    SDL_DestroyWindow(window.release());
    SDL_Quit();
}
