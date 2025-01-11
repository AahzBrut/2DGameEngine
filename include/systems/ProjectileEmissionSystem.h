#pragma once
#include <glm/glm.hpp>
#include "asset_manager/AssetManager.h"
#include "components/DamageComponent.h"
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
        RequireComponent<VelocityComponent>();
        RequireComponent<TransformComponent>();
        RequireComponent<DirectionComponent>();
    }

    void Update() const {
        if (!assetManager || !registry) return;
        const auto &bulletSprite = assetManager->GetTexture("bullet");
        for (const auto entity: GetSystemEntities()) {
            const auto &transform = entity.GetComponent<TransformComponent>();
            const auto &direction = entity.GetComponent<DirectionComponent>();

            if (auto &projectile = entity.GetComponent<ProjectileEmitterComponent>();
                projectile.autoShoot &&
                SDL_GetTicks() - projectile.lastShotTime >= projectile.cooldown) {
                projectile.lastShotTime = static_cast<int>(SDL_GetTicks());
                const auto velocity = direction.direction * projectile.velocity;
                const auto &emitterSprite = entity.GetComponent<SpriteComponent>();

                registry->CreateEntity()
                        .AddComponent<DamageComponent>(projectile.bulletDamage)
                        .AddComponent<TransformComponent>(transform.position + glm::vec2{
                                                              static_cast<float>(emitterSprite.rect.w) *
                                                              transform.scale.x / 2,
                                                              static_cast<float>(emitterSprite.rect.h) *
                                                              transform.scale.y / 2
                                                          } - glm::vec2{
                                                              static_cast<float>(bulletSprite.width) *
                                                              transform.scale.x / 2,
                                                              static_cast<float>(bulletSprite.height) *
                                                              transform.scale.y / 2
                                                          }, transform.scale, 0)
                        .AddComponent<VelocityComponent>(velocity)
                        .AddComponent<BoxColliderComponent>(4, 4, glm::vec2{0}, projectile.collisionLayer)
                        .AddComponent<SpriteComponent>(bulletSprite, bulletSprite.TextureRect(), 4)
                        .AddComponent<TempEntityComponent>(projectile.lifetime);
            }
        }
    }
};
