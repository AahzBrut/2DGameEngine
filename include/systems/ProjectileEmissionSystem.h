#pragma once
#include "asset_manager/AssetManager.h"
#include "components/TempEntityComponent.h"
#include "ecs/ECS.h"


class ProjectileEmissionSystem : public System {
    Registry *registry;
    AssetManager *assetManager;

public:
    explicit ProjectileEmissionSystem(
        const Unique<Registry> &registry,
        const Unique<AssetManager> &assetManager)
        : registry{registry.get()}, assetManager{assetManager.get()} {
        RequireComponent<ProjectileEmitterComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update() const {
        if (!assetManager || !registry) return;
        const auto &bulletSprite = assetManager->GetTexture("bullet");
        for (const auto entity: GetSystemEntities()) {
            const auto &transform = entity.GetComponent<TransformComponent>();

            if (auto &projectile = entity.GetComponent<ProjectileEmitterComponent>();
                SDL_GetTicks() - projectile.lastShotTime >= projectile.cooldown) {
                projectile.lastShotTime = static_cast<int>(SDL_GetTicks());
                const auto &emitterSprite = entity.GetComponent<SpriteComponent>();
                registry->CreateEntity()
                        .AddComponent<TransformComponent>(transform.position + glm::vec2{
                                                              static_cast<float>(emitterSprite.sprite.width) * transform.scale.x / 2,
                                                              static_cast<float>(emitterSprite.sprite.height) * transform.scale.y / 2
                                                          }, transform.scale, 0)
                        .AddComponent<VelocityComponent>(projectile.velocity)
                        .AddComponent<BoxColliderComponent>(4, 4, glm::vec2{0}, projectile.collisionLayer)
                        .AddComponent<SpriteComponent>(bulletSprite, bulletSprite.TextureRect(), 3)
                        .AddComponent<TempEntityComponent>(projectile.lifetime);
            }
        }
    }
};
