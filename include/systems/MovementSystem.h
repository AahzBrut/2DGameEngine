#pragma once
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "ecs/ECS.h"


class MovementSystem : public System {
public:
    MovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<VelocityComponent>();
    }

    void Update(const float deltaTime) const {
        for (const auto& entity: GetSystemEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            const auto& velocity = entity.GetComponent<VelocityComponent>();
            transform.position += velocity.velocity * deltaTime;

            LOG("Entity {} moved to ({}, {})", entity.GetId(), transform.position.x, transform.position.y);
        }
    }
};
