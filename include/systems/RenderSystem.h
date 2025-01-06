#pragma once
#include "components/TransformComponent.h"
#include "ecs/ECS.h"


class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Render(const Unique<SDL_Renderer>& renderer) const {
        for (const auto &entity: GetSystemEntities()) {
            const auto &transform = entity.GetComponent<TransformComponent>();
            const auto &sprite = entity.GetComponent<SpriteComponent>();

            SDL_Rect srcRect = {0,0, sprite.sprite.width, sprite.sprite.height};
            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                static_cast<int>(sprite.size.x * transform.scale.x),
                static_cast<int>(sprite.size.y * transform.scale.y),
            };

            SDL_RenderCopy(renderer.get(), sprite.sprite.texture.get(), &srcRect, &dstRect);
        }
    }
};
