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

            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                static_cast<int>(static_cast<float>(sprite.rect.w) * transform.scale.x),
                static_cast<int>(static_cast<float>(sprite.rect.h) * transform.scale.y),
            };

            SDL_RenderCopy(renderer.get(), sprite.sprite.texture.get(), &sprite.rect, &dstRect);
        }
    }
};
