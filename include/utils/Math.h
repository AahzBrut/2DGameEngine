#pragma once
#include <SDL2/SDL_rect.h>

#include "components/BoxColliderComponent.h"
#include "components/TransformComponent.h"


inline SDL_Rect makeRect(const BoxColliderComponent& collider, const TransformComponent& transform) {
    return SDL_Rect{
        static_cast<int>(transform.position.x + collider.offset.x * transform.scale.x),
        static_cast<int>(transform.position.y + collider.offset.y * transform.scale.y),
        static_cast<int>(static_cast<float>(collider.width) * transform.scale.x),
        static_cast<int>(static_cast<float>(collider.height) * transform.scale.y)
    };
}

inline bool isIntersects(const SDL_Rect &firstRect, const SDL_Rect &secondRect) {
    return firstRect.x < secondRect.x + secondRect.w &&
           firstRect.x + firstRect.w > secondRect.x &&
           firstRect.y < secondRect.y + secondRect.h &&
           firstRect.y + firstRect.h > secondRect.y;
}
