#pragma once
#include "data/CollisionLayer.h"
#include "glm/vec2.hpp"


struct BoxColliderComponent {
    int width, height;
    glm::vec2 offset;
    CollisionLayer collisionLayer;

    explicit BoxColliderComponent(
        const int width = 0,
        const int height = 0,
        const glm::vec2 &offset = glm::vec2{0},
        const CollisionLayer collisionLayer = CollisionLayer::None)
        : width{width},
          height{height},
          offset{offset},
          collisionLayer{collisionLayer} {}
};
