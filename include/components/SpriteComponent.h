#pragma once

#include <utility>

#include "ecs/ECS.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"


struct SpriteComponent : Component<SpriteComponent> {
    Sprite sprite;
    SDL_Rect rect;
    glm::vec4 color;

    explicit SpriteComponent(
        Sprite sprite = Sprite{},
        const SDL_Rect& rect = {},
        const glm::vec4 color = glm::vec4(0, 0, 0, 255))
        : sprite{std::move(sprite)}, rect{rect}, color{color} {}
};
