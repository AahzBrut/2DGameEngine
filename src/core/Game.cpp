#include "core/Game.h"

#include <format>
#include <fstream>
#include <iostream>
#include <ostream>
#include <SDL2/SDL.h>

#include "components/AnimationComponent.h"
#include "components/SpriteComponent.h"
#include "core/LevelLoader.h"
#include "core/Logger.h"
#include "systems/AnimationSystem.h"
#include "systems/CameraMovementSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/DamageSystem.h"
#include "systems/HealthBarSystem.h"
#include "systems/KeyboardControlSystem.h"
#include "systems/MovementSystem.h"
#include "systems/ProjectileEmissionSystem.h"
#include "systems/RenderColliderSystem.h"
#include "systems/RenderLabelSystem.h"
#include "systems/RenderSystem.h"
#include "systems/SoundPlaySystem.h"
#include "systems/TempEntitiesRemovalSystem.h"


int Game::windowWidth = 0;
int Game::windowHeight = 0;
int Game::mapWidth = 0;
int Game::mapHeight = 0;

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

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init error: " << SDL_GetError() << std::endl;
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

    audioManager->Initialize();

    camera = {0, 0, windowWidth, windowHeight};

    isRunning = true;
}

void Game::Setup() {
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<KeyboardControlSystem>(eventBus);
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmissionSystem>(registry, assetManager, eventBus);
    registry->AddSystem<TempEntitiesRemovalSystem>(registry);
    registry->AddSystem<DamageSystem>(eventBus);
    registry->AddSystem<SoundPlaySystem>(audioManager, eventBus);
    registry->AddSystem<RenderLabelSystem>();
    registry->AddSystem<HealthBarSystem>(assetManager);

    lua.open_libraries(sol::lib::base, sol::lib::math);
    const LevelLoader levelLoader{lua, assetManager, renderer, registry};
    levelLoader.LoadLevel(1);
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
                if (event.key.keysym.sym == SDLK_BACKQUOTE) {
                    isDebugMode = !isDebugMode;
                }
                eventBus->EmitEvent<KeyPressedEvent>(SDL_KEYDOWN, event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                eventBus->EmitEvent<KeyPressedEvent>(SDL_KEYUP, event.key.keysym.sym);
                break;
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
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileEmissionSystem>().Update();
    registry->GetSystem<TempEntitiesRemovalSystem>().Update();
    registry->GetSystem<SoundPlaySystem>().Update();

    lastFrameTicks = SDL_GetTicks();
}

void Game::Render() const {
    SDL_SetRenderDrawColor(renderer.get(), 63, 63, 63, 255);
    SDL_RenderClear(renderer.get());
    // Render all game objects and UI

    registry->GetSystem<RenderSystem>().Render(renderer, camera);
    if (isDebugMode) registry->GetSystem<RenderColliderSystem>().Render(renderer, camera);
    registry->GetSystem<RenderLabelSystem>().Render(renderer);
    registry->GetSystem<HealthBarSystem>().Render(renderer, camera);

    SDL_RenderPresent(renderer.get());
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer.release());
    SDL_DestroyWindow(window.release());
    audioManager->Shutdown();
    SDL_Quit();
}
