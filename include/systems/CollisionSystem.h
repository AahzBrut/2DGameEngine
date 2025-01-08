#pragma once
#include "components/BoxColliderComponent.h"
#include "components/TransformComponent.h"
#include "ecs/ECS.h"
#include "events/CollisionEvent.h"


class CollisionSystem : public System {
public:
    explicit CollisionSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    static bool IsIntersects(const SDL_Rect &firstRect, const SDL_Rect &secondRect) {
        return firstRect.x < secondRect.x + secondRect.w &&
               firstRect.x + firstRect.w > secondRect.x &&
               firstRect.y < secondRect.y + secondRect.h &&
               firstRect.y + firstRect.h > secondRect.y;
    }

    void Update(const Unique<EventBus>& eventBus) const {
        auto entities = GetSystemEntities();

        for (auto it = entities.begin(); it != entities.end(); ++it) {
            const auto currentEntity = *it;
            const auto& firstCollider = currentEntity.GetComponent<BoxColliderComponent>();
            const auto& firstTransform = currentEntity.GetComponent<TransformComponent>();
            const auto firstRect = SDL_Rect{
                static_cast<int>(firstTransform.position.x + firstCollider.offset.x * firstTransform.scale.x),
                static_cast<int>(firstTransform.position.y + firstCollider.offset.y * firstTransform.scale.y),
                static_cast<int>(static_cast<float>(firstCollider.width) * firstTransform.scale.x),
                static_cast<int>(static_cast<float>(firstCollider.height) * firstTransform.scale.y)
            };
            for (auto inner = it + 1; inner != entities.end(); ++inner) {
                const auto otherEntity = *inner;
                const auto& secondCollider = otherEntity.GetComponent<BoxColliderComponent>();
                const auto& secondTransform = otherEntity.GetComponent<TransformComponent>();
                const auto secondRect = SDL_Rect{
                    static_cast<int>(secondTransform.position.x + secondCollider.offset.x * secondTransform.scale.x),
                    static_cast<int>(secondTransform.position.y + secondCollider.offset.y * secondTransform.scale.y),
                    static_cast<int>(static_cast<float>(secondCollider.width) * secondTransform.scale.x),
                    static_cast<int>(static_cast<float>(secondCollider.height) * secondTransform.scale.y)
                };

                if (IsIntersects(firstRect, secondRect)) {
                    eventBus->EmitEvent<CollisionEvent>(currentEntity, otherEntity);
                }
            }
        }
    }
};
