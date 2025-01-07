#pragma once
#include "components/BoxColliderComponent.h"
#include "components/TransformComponent.h"
#include "ecs/ECS.h"

class RenderColliderSystem : public System {
public:
    RenderColliderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Render(const Unique<SDL_Renderer> &renderer) const {
        for (const auto &entity: GetSystemEntities()) {
            const auto &transform = entity.GetComponent<TransformComponent>();
            const auto &collider = entity.GetComponent<BoxColliderComponent>();

            const auto colliderRect = SDL_Rect{
                static_cast<int>(transform.position.x + collider.offset.x * transform.scale.x),
                static_cast<int>(transform.position.y + collider.offset.y * transform.scale.y),
                static_cast<int>(static_cast<float>(collider.width) * transform.scale.x),
                static_cast<int>(static_cast<float>(collider.height) *  transform.scale.y)
            };
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 0, 0);
            SDL_RenderDrawRect(renderer.get(), &colliderRect);
        }
    }
};
