#pragma once
#include "components/TransformComponent.h"
#include "ecs/ECS.h"


class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Render(SDL_Renderer *renderer) const {
        for (const auto &entity: GetSystemEntities()) {
            const auto &transform = entity.GetComponent<TransformComponent>();
            const auto &sprite = entity.GetComponent<SpriteComponent>();
            SDL_Rect renderQuad{static_cast<int>(transform.position.x), static_cast<int>(transform.position.y), static_cast<int>(sprite.size.x), static_cast<int>(sprite.size.x)};
            SDL_SetRenderDrawColor(renderer, sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a);
            SDL_RenderFillRect(renderer, &renderQuad);
        }
    }
};
