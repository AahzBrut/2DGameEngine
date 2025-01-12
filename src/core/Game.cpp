#include "core/Game.h"

#include <format>
#include <fstream>
#include <iostream>
#include <ostream>
#include <SDL2/SDL.h>

#include "components/AnimationComponent.h"
#include "components/BoxColliderComponent.h"
#include "components/CameraFollowComponent.h"
#include "components/DirectionComponent.h"
#include "components/HealthComponent.h"
#include "components/ProjectileEmitterComponent.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "core/Logger.h"
#include "glm/vec2.hpp"
#include "systems/AnimationSystem.h"
#include "systems/CameraMovementSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/DamageSystem.h"
#include "systems/KeyboardControlSystem.h"
#include "systems/MovementSystem.h"
#include "systems/ProjectileEmissionSystem.h"
#include "systems/RenderColliderSystem.h"
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

// ReSharper disable once CppMemberFunctionMayBeConst
void Game::LoadLevel([[maybe_unused]] int level) {
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<KeyboardControlSystem>(eventBus);
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmissionSystem>(registry, assetManager);
    registry->AddSystem<TempEntitiesRemovalSystem>(registry);
    registry->AddSystem<DamageSystem>(eventBus);
    registry->AddSystem<SoundPlaySystem>(audioManager);

    assetManager->LoadTexture(renderer, "tank", "./assets/images/tank-panther-right.png");
    assetManager->LoadTexture(renderer, "truck", "./assets/images/truck-ford-right.png");
    assetManager->LoadTexture(renderer, "jungle-map", "./assets/tilemaps/jungle.png");
    assetManager->LoadTexture(renderer, "chopper", "./assets/images/chopper-spritesheet.png");
    assetManager->LoadTexture(renderer, "radar", "./assets/images/radar.png");
    assetManager->LoadTexture(renderer, "bullet", "./assets/images/bullet.png");

    assetManager->LoadSoundEffect("helicopter-sound", "./assets/sounds/explosion4.wav");
    assetManager->LoadMusic("main-music-theme", "./assets/music/Abnormal Circumstances.mp3");

    //Mix_PlayChannel(-1, assetManager->GetSoundEffect("helicopter-sound").get(), 0);
    //audioManager->PlayMusic("main-music-theme", -1);
    //audioManager->PlaySound("helicopter-sound", .9f);

    const auto &jungleMapSprite = assetManager->GetTexture("jungle-map");
    mapHeight = 20;
    std::fstream mapFile;
    mapWidth = 25;
    constexpr auto tileScale = 2.0;
    constexpr auto tileSize = 32;
    mapFile.open("./assets/tilemaps/jungle.map");
    for (auto y = 0; y < mapHeight; y++) {
        for (auto x = 0; x < mapWidth; x++) {
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
    mapHeight = mapHeight * tileSize * static_cast<int>(tileScale);
    mapWidth = mapWidth * tileSize * static_cast<int>(tileScale);

    mapFile.close();

    const auto &tankSprite = assetManager->GetTexture("tank");
    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{0}, glm::vec2{2, 2}, 0.0)
            .AddComponent<VelocityComponent>(glm::vec2{0})
            .AddComponent<SpriteComponent>(tankSprite, tankSprite.TextureRect(), 2)
            .AddComponent<HealthComponent>(20, 20)
            .AddComponent<DirectionComponent>()
            .AddComponent<ProjectileEmitterComponent>(glm::vec2{50, 50}, CollisionLayer::EnemyBullet)
            .AddComponent<BoxColliderComponent>(26, 16, glm::vec2(3, 8), CollisionLayer::Enemy);

    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{200, 200}, glm::vec2{2, 2}, 0.0)
            .AddComponent<VelocityComponent>(glm::vec2{0})
            .AddComponent<SpriteComponent>(tankSprite, tankSprite.TextureRect(), 2)
            .AddComponent<HealthComponent>(20, 20)
            .AddComponent<DirectionComponent>()
            .AddComponent<ProjectileEmitterComponent>(glm::vec2{50, 50}, CollisionLayer::EnemyBullet)
            .AddComponent<BoxColliderComponent>(26, 16, glm::vec2(3, 8), CollisionLayer::Enemy);

    const auto &truckSprite = assetManager->GetTexture("truck");
    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{400, 0}, glm::vec2{2, 2}, 0.0)
            .AddComponent<VelocityComponent>(glm::vec2{0})
            .AddComponent<SpriteComponent>(truckSprite, truckSprite.TextureRect(), 1)
            .AddComponent<HealthComponent>(20, 20)
            .AddComponent<DirectionComponent>()
            .AddComponent<ProjectileEmitterComponent>(glm::vec2{50, 50}, CollisionLayer::EnemyBullet)
            .AddComponent<BoxColliderComponent>(32, 32, glm::vec2{0}, CollisionLayer::Enemy);

    const auto &chopperSprite = assetManager->GetTexture("chopper");
    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{400, 400}, glm::vec2{2, 2}, 0.0)
            .AddComponent<VelocityComponent>(glm::vec2{0, 0})
            .AddComponent<SpriteComponent>(chopperSprite, chopperSprite.TextureRect(), 3)
            .AddComponent<BoxColliderComponent>(32, 32, glm::vec2{0}, CollisionLayer::Player)
            .AddComponent<HealthComponent>(100, 100)
            .AddComponent<DirectionComponent>(glm::vec2{0, -1})
            .AddComponent<ProjectileEmitterComponent>(glm::vec2{100, 100}, CollisionLayer::PlayerBullet, 500, 10000, 10,
                                                      false)
            .AddComponent<KeyboardControlComponent>(glm::vec2{0, -80}, glm::vec2{80, 0}, glm::vec2{0, 80},
                                                    glm::vec2{-80, 0})
            .AddComponent<CameraFollowComponent>()
            .AddComponent<AnimationComponent>(List{
                                                  List{
                                                      SDL_Rect{0, 0, 32, 32},
                                                      SDL_Rect{32, 0, 32, 32},
                                                  },
                                                  List{
                                                      SDL_Rect{0, 32, 32, 32},
                                                      SDL_Rect{32, 32, 32, 32},
                                                  },
                                                  List{
                                                      SDL_Rect{0, 64, 32, 32},
                                                      SDL_Rect{32, 64, 32, 32},
                                                  },
                                                  List{
                                                      SDL_Rect{0, 96, 32, 32},
                                                      SDL_Rect{32, 96, 32, 32},
                                                  },
                                              }, 0, 0, 8.f, true, 0);

    const auto &radarSprite = assetManager->GetTexture("radar");
    registry->CreateEntity()
            .AddComponent<TransformComponent>(glm::vec2{windowWidth - 72, 8}, glm::vec2{1, 1}, 0.0)
            .AddComponent<SpriteComponent>(radarSprite, radarSprite.TextureRect(), 3,
                                           SDL_Color{255, 255, 255, 255}, true)
            .AddComponent<AnimationComponent>(List<List<SDL_Rect> >{
                                                  List{
                                                      SDL_Rect{0, 0, 64, 64},
                                                      SDL_Rect{64, 0, 64, 64},
                                                      SDL_Rect{128, 0, 64, 64},
                                                      SDL_Rect{192, 0, 64, 64},
                                                      SDL_Rect{256, 0, 64, 64},
                                                      SDL_Rect{320, 0, 64, 64},
                                                      SDL_Rect{384, 0, 64, 64},
                                                      SDL_Rect{448, 0, 64, 64},
                                                  }
                                              }, 0, 0, 8.f, true, 0);
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
    registry->GetSystem<SoundPlaySystem>().Update(camera);

    lastFrameTicks = SDL_GetTicks();
}

void Game::Render() const {
    SDL_SetRenderDrawColor(renderer.get(), 63, 63, 63, 255);
    SDL_RenderClear(renderer.get());
    // Render all game objects and UI

    registry->GetSystem<RenderSystem>().Render(renderer, camera);
    if (isDebugMode) registry->GetSystem<RenderColliderSystem>().Render(renderer, camera);

    SDL_RenderPresent(renderer.get());
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer.release());
    SDL_DestroyWindow(window.release());
    audioManager->Shutdown();
    SDL_Quit();
}
