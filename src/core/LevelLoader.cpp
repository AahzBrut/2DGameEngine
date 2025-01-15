#include "core/LevelLoader.h"

#include <fstream>

#include "components/AnimationComponent.h"
#include "components/BoxColliderComponent.h"
#include "components/CameraFollowComponent.h"
#include "components/DirectionComponent.h"
#include "components/HealthComponent.h"
#include "components/KeyboardControlComponent.h"
#include "components/LabelComponent.h"
#include "components/ProjectileEmitterComponent.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "core/Game.h"
#include "core/SmartPointers.h"
#include "glm/vec2.hpp"
#include "sol/sol.hpp"


LevelLoader::LevelLoader(
    sol::state &lua,
    Unique<AssetManager> &assetManager,
    Unique<SDL_Renderer> &renderer,
    Unique<Registry> &registry)
    : lua{lua}, assetManager{assetManager}, renderer{renderer}, registry{registry} {}

void LevelLoader::LoadLevel(int levelId) const {
    using Vec2 = glm::vec2;
    const auto levelPath = std::format("./assets/scripts/Level-{}.lua", levelId);

    if (const auto level = lua.load_file(levelPath); !level.valid()) {
        const sol::error err = level;
        const auto errorMessage = err.what();
        LOG_ERR("Failed to load lua script {}: {}", levelPath, errorMessage);

        return;
    }

    lua.script_file(levelPath);

    const sol::table table = lua["Level"];
    const sol::table assets = table["assets"];

    auto i = 0;
    while (true) {
        sol::optional<sol::table> asset = assets[++i];
        if (!asset.has_value()) break;
        const auto &assetValue = asset.value();
        const auto assetType = assetValue["type"].get<std::string>();
        if (assetType == "texture") {
            assetManager.get()->LoadTexture(renderer, assetValue["id"], assetValue["file"]);
            LOG("Loaded texture: '{}' ({})", assetValue["id"].get<std::string>(),
                assetValue["file"].get<std::string>());
        }
        if (assetType == "font") {
            assetManager.get()->LoadFont(assetValue["id"], assetValue["file"], assetValue["fontSize"]);
            LOG("Loaded font: '{}' ({})", assetValue["id"].get<std::string>(), assetValue["file"].get<std::string>());
        }
        if (assetType == "music") {
            assetManager.get()->LoadMusic(assetValue["id"], assetValue["file"]);
            LOG("Loaded music: '{}' ({})", assetValue["id"].get<std::string>(), assetValue["file"].get<std::string>());
        }
        if (assetType == "soundEffect") {
            assetManager.get()->LoadSoundEffect(assetValue["id"], assetValue["file"]);
            LOG("Loaded sound effect: '{}' ({})", assetValue["id"].get<std::string>(),
                assetValue["file"].get<std::string>());
        }
    }

    const sol::table tilemap = table["tilemap"];

    const auto &jungleMapSprite = assetManager->GetTexture(tilemap["textureAssetId"]);

    Game::mapHeight = tilemap["numRows"].get<int>();
    Game::mapWidth = tilemap["numCols"].get<int>();
    const auto tileScale = tilemap["scale"].get<float>();
    const auto tileSize = tilemap["tileSize"].get<int>();

    std::fstream mapFile;
    mapFile.open(tilemap["mapFile"].get<std::string>());
    for (auto y = 0; y < Game::mapHeight; y++) {
        for (auto x = 0; x < Game::mapWidth; x++) {
            char character;
            mapFile.get(character);
            const auto srcRectY = tileSize * std::atoi(&character); // NOLINT(*-err34-c)
            mapFile.get(character);
            const auto srcRectX = tileSize * std::atoi(&character); // NOLINT(*-err34-c)
            mapFile.ignore();
            const auto position = Vec2{x, y} * (tileScale * static_cast<float>(tileSize));
            const auto scale = Vec2{tileScale};
            registry->CreateEntity()
                    .AddComponent<TransformComponent>(position, scale, 0.0)
                    .AddComponent<SpriteComponent>(jungleMapSprite, SDL_Rect{srcRectX, srcRectY, tileSize, tileSize},
                                                   0);
        }
    }
    Game::mapHeight = Game::mapHeight * tileSize * static_cast<int>(tileScale);
    Game::mapWidth = Game::mapWidth * tileSize * static_cast<int>(tileScale);

    mapFile.close();

    const sol::table entities = table["entities"];
    i = 0;
    while (true) {
        sol::optional<sol::table> entityDef = entities[++i];
        if (!entityDef.has_value()) break;

        const auto &entityDefValue = entityDef.value();

        auto entity = registry->CreateEntity();

        if (const auto components = entityDefValue["components"]; components.valid()) {
            if (const auto transformDef = components["transform"]; transformDef.valid()) {
                LoadTransformComponent(entity, transformDef);
            }
            if (const auto spriteDef = components["sprite"]; spriteDef.valid()) {
                LoadSpriteComponent(entity, spriteDef);
            }
            if (const auto animationDef = components["animation"]; animationDef.valid()) {
                entity.AddComponent<AnimationComponent>(
                    GetAnimationFrames(animationDef["frames"]),
                    animationDef["frameRate"]
                );
            }
            if (const auto velocityDef = components["velocity"]; velocityDef.valid()) {
                entity.AddComponent<VelocityComponent>(
                    Vec2{velocityDef["velocity"]["x"], velocityDef["velocity"]["y"]}
                );
            }
            if (const auto boxColliderDef = components["boxCollider"]; boxColliderDef.valid()) {
                entity.AddComponent<BoxColliderComponent>(
                    boxColliderDef["width"],
                    boxColliderDef["height"],
                    Vec2{boxColliderDef["offset"]["x"], boxColliderDef["offset"]["y"]},
                    CollisionLayerFromString(boxColliderDef["collisionLayer"])
                );
            }
            if (const auto cameraFollowDef = components["cameraFollow"]; cameraFollowDef.valid()) {
                entity.AddComponent<CameraFollowComponent>();
            }
            if (const auto directionDef = components["direction"]; directionDef.valid()) {
                entity.AddComponent<DirectionComponent>(Vec2{
                    directionDef["direction"]["x"], directionDef["direction"]["y"]
                });
            }
            if (const auto keyboardControlDef = components["keyboardControl"]; keyboardControlDef.valid()) {
                entity.AddComponent<KeyboardControlComponent>(
                    Vec2{keyboardControlDef["upVector"]["x"], keyboardControlDef["upVector"]["y"]},
                    Vec2{keyboardControlDef["rightVector"]["x"], keyboardControlDef["rightVector"]["y"]},
                    Vec2{keyboardControlDef["downVector"]["x"], keyboardControlDef["downVector"]["y"]},
                    Vec2{keyboardControlDef["leftVector"]["x"], keyboardControlDef["leftVector"]["y"]}
                );
            }
            if (const auto projectileEmitterDef = components["projectileEmitter"]; projectileEmitterDef.valid()) {
                entity.AddComponent<ProjectileEmitterComponent>(
                    Vec2{projectileEmitterDef["projectileVelocity"]["x"], projectileEmitterDef["projectileVelocity"]["x"]},
                    CollisionLayerFromString(projectileEmitterDef["collisionLayer"]),
                    projectileEmitterDef["coolDown"],
                    projectileEmitterDef["lifeTime"],
                    projectileEmitterDef["bulletDamage"],
                    projectileEmitterDef["autoShoot"].get<bool>()
                );
            }
            if (const auto healthDef = components["health"]; healthDef.valid()) {
                entity.AddComponent<HealthComponent>(
                    healthDef["health"],
                    healthDef["maxHealth"]
                );
            }
        }
    }

    //
    // const auto &tankSprite = assetManager->GetTexture("tank");
    // registry->CreateEntity()
    //         .AddComponent<HealthComponent>(20, 20)
    //         .AddComponent<ProjectileEmitterComponent>(glm::vec2{50, 50}, CollisionLayer::EnemyBullet)
    //
    // registry->CreateEntity()
    //         .AddComponent<HealthComponent>(20, 20)
    //         .AddComponent<ProjectileEmitterComponent>(glm::vec2{50, 50}, CollisionLayer::EnemyBullet)
    //
    // const auto &truckSprite = assetManager->GetTexture("truck");
    // registry->CreateEntity()
    //         .AddComponent<HealthComponent>(20, 20)
    //         .AddComponent<ProjectileEmitterComponent>(glm::vec2{50, 50}, CollisionLayer::EnemyBullet)
    //
    // const auto &chopperSprite = assetManager->GetTexture("chopper");
    // registry->CreateEntity()
    //         .AddComponent<HealthComponent>(100, 100)
    //         .AddComponent<ProjectileEmitterComponent>(
    //             glm::vec2{100, 100},
    //             CollisionLayer::PlayerBullet,
    //             500,
    //             10000,
    //             10,
    //             false)
    //         .AddComponent<AnimationComponent>(List{
    //                                               List{
    //                                                   SDL_Rect{0, 0, 32, 32},
    //                                                   SDL_Rect{32, 0, 32, 32},
    //                                               },
    //                                               List{
    //                                                   SDL_Rect{0, 32, 32, 32},
    //                                                   SDL_Rect{32, 32, 32, 32},
    //                                               },
    //                                               List{
    //                                                   SDL_Rect{0, 64, 32, 32},
    //                                                   SDL_Rect{32, 64, 32, 32},
    //                                               },
    //                                               List{
    //                                                   SDL_Rect{0, 96, 32, 32},
    //                                                   SDL_Rect{32, 96, 32, 32},
    //                                               },
    //                                           }, 0, 0, 8.f, true, 0);
    //
    // const auto &radarSprite = assetManager->GetTexture("radar");
    // registry->CreateEntity()
    //         .AddComponent<TransformComponent>(glm::vec2{Game::windowWidth - 72, 8}, glm::vec2{1, 1}, 0.0)
    //         .AddComponent<SpriteComponent>(radarSprite, radarSprite.TextureRect(), 3,
    //                                        SDL_Color{255, 255, 255, 255}, true)
    //         .AddComponent<AnimationComponent>(List<List<SDL_Rect> >{
    //                                               List{
    //                                                   SDL_Rect{0, 0, 64, 64},
    //                                                   SDL_Rect{64, 0, 64, 64},
    //                                                   SDL_Rect{128, 0, 64, 64},
    //                                                   SDL_Rect{192, 0, 64, 64},
    //                                                   SDL_Rect{256, 0, 64, 64},
    //                                                   SDL_Rect{320, 0, 64, 64},
    //                                                   SDL_Rect{384, 0, 64, 64},
    //                                                   SDL_Rect{448, 0, 64, 64},
    //                                               }
    //                                           }, 0, 0, 8.f, true, 0);
    // registry->CreateEntity()
    //         .AddComponent<LabelComponent>(
    //             glm::vec2{200, 0},
    //             "Test label",
    //             assetManager.get()->GetFont("charriot-font"),
    //             SDL_Color{255, 255, 255, 255});
}

List<List<SDL_Rect> > LevelLoader::GetAnimationFrames(const sol::table &sourceTable) {
    List<List<SDL_Rect> > animationFrames;
    auto sequenceIndex = 0;
    while (true) {
        const auto sequenceDef = sourceTable[++sequenceIndex];
        if (!sequenceDef.valid()) return animationFrames;
        auto frameIndex = 0;
        List<SDL_Rect> frames;
        while (true) {
            const auto frameDef = sequenceDef[++frameIndex];
            if (!frameDef.valid()) break;
            frames.push_back(SDL_Rect{
                frameDef["x"].get<int>(),
                frameDef["y"].get<int>(),
                frameDef["w"].get<int>(),
                frameDef["h"].get<int>()
            });
        }
        animationFrames.push_back(frames);
    }
}

void LevelLoader::LoadTransformComponent(Entity entity, const sol::table &transformDef) {
    using Vec2 = glm::vec2;
    const auto positionDef = transformDef["position"];
    const auto scaleDef = transformDef["scale"];
    entity.AddComponent<TransformComponent>(
        Vec2{positionDef["x"].get<float>(), positionDef["y"].get<float>()},
        Vec2{scaleDef["x"].get<float>(), scaleDef["y"].get<float>()},
        transformDef["rotation"].get<float>()
    );
}

void LevelLoader::LoadSpriteComponent(Entity entity, const sol::table &spriteDef) const {
    const auto sprite = assetManager->GetTexture(spriteDef["textureAssetId"]);
    entity.AddComponent<SpriteComponent>(
        sprite,
        sprite.TextureRect(),
        spriteDef["zIndex"].get<int>()
    );
}
