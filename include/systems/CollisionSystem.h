#pragma once
#include "components/BoxColliderComponent.h"
#include "components/TransformComponent.h"
#include "ecs/ECS.h"
#include "events/CollisionEvent.h"
#include "utils/Math.h"


class CollisionSystem : public System {
public:
    explicit CollisionSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update(const Unique<EventBus> &eventBus) const {
        auto entities = GetSystemEntities();

        for (auto it = entities.begin(); it != entities.end(); ++it) {
            const auto currentEntity = *it;
            const auto &firstCollider = currentEntity.GetComponent<BoxColliderComponent>();
            const auto &firstTransform = currentEntity.GetComponent<TransformComponent>();

            const auto firstRect = makeRect(firstCollider, firstTransform);
            for (auto inner = it + 1; inner != entities.end(); ++inner) {
                const auto otherEntity = *inner;
                const auto &secondCollider = otherEntity.GetComponent<BoxColliderComponent>();
                const auto &secondTransform = otherEntity.GetComponent<TransformComponent>();

                const auto secondRect = makeRect(secondCollider, secondTransform);
                if (LayersCollisionSettings::IsLayersCollides(firstCollider.collisionLayer, secondCollider.collisionLayer)) {
                    if (isIntersects(firstRect, secondRect)) {
                        eventBus->EmitEvent<CollisionEvent>(currentEntity, otherEntity);
                        LOG("Collision detected");
                    }
                }
            }
        }
    }
};
