#pragma once

#include <utility>

#include "ecs/ECS.h"


struct SpriteComponent : Component<SpriteComponent> {
    Sprite sprite;
    SDL_Rect rect;
    SDL_Color color;
    int zIndex;

    explicit SpriteComponent(
        Sprite sprite = Sprite{},
        const SDL_Rect &rect = {},
        const SDL_Color &color = SDL_Color{255, 255, 255, 255},
        const int zIndex = 0)
        : sprite{std::move(sprite)}, rect{rect}, color{color}, zIndex{zIndex} {}
};
