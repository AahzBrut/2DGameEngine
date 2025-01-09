#pragma once
#include "data/CollisionLayer.h"


struct ProjectileEmitterComponent {
    glm::vec2 velocity;
    CollisionLayer collisionLayer;
    int cooldown;
    int lifetime;
    int bulletDamage;
    int lastShotTime{static_cast<int>(SDL_GetTicks())};

    explicit ProjectileEmitterComponent(
        const glm::vec2 &velocity = glm::vec2{0},
        const CollisionLayer collisionLayer = CollisionLayer::None,
        const int cooldown = 2000,
        const int lifetime = 10000,
        const int bulletDamage = 10)
        : velocity{velocity},
          collisionLayer{collisionLayer},
          cooldown{cooldown},
          lifetime{lifetime},
          bulletDamage{bulletDamage} {}
};
