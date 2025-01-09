#pragma once
#include "components/TempEntityComponent.h"
#include "ecs/ECS.h"


class TempEntitiesRemovalSystem : public System {
    Registry *registry;

public:
    explicit TempEntitiesRemovalSystem(const Unique<Registry> &registry)
        : registry{registry.get()} {
        RequireComponent<TempEntityComponent>();
    }

    void Update() const {
        for (const auto entity: GetSystemEntities()) {
            const auto& temp = registry->GetComponent<TempEntityComponent>(entity);
            if (const auto timeElapsed = static_cast<int>(SDL_GetTicks()) - temp.birthTime;
                timeElapsed > temp.lifeSpanLeft) {
                entity.Destroy();
            }
        }
    }
};
