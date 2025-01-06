#pragma once

#include <utility>

#include "ecs/ECS.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"


struct SpriteComponent : Component<SpriteComponent> {
    Sprite sprite;
    glm::vec2 size;
    glm::vec4 color;

    explicit SpriteComponent(
        Sprite sprite = Sprite{},
        const glm::vec2 size = glm::vec2(0, 0),
        const glm::vec4 color = glm::vec4(0, 0, 0, 255))
        : sprite{std::move(sprite)}, size{size}, color{color} {}
};
