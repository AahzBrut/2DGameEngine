#pragma once

#include <utility>

#include "ecs/ECS.h"


struct SpriteComponent : Component<SpriteComponent> {
    Sprite sprite;
    SDL_Rect rect;
    int zIndex;
    SDL_Color color;

    explicit SpriteComponent(
        Sprite sprite = Sprite{},
        const SDL_Rect &rect = {},
        const int zIndex = 0,
        const SDL_Color &color = SDL_Color{255, 255, 255, 255})
        : sprite{std::move(sprite)}, rect{rect}, zIndex{zIndex}, color{color} {}
};
