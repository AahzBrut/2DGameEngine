#pragma once

#include "components/DamageComponent.h"
#include "components/HealthComponent.h"
#include "ecs/ECS.h"
#include "events/CollisionEvent.h"
#include "event_bus/EventBus.h"


class DamageSystem : public System {
    public:
    explicit DamageSystem(const Unique<EventBus> &eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnEntitiesCollides);
  }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    void OnEntitiesCollides(CollisionEvent &event) { // NOLINT(*-convert-member-functions-to-static)
        const auto projectile = event.firstEntity.HasComponent<DamageSystem>() ? event.firstEntity : event.secondEntity;
        const auto unit = event.firstEntity.HasComponent<HealthComponent>() ? event.firstEntity : event.secondEntity;
        const auto damage = projectile.GetComponent<DamageComponent>().damage;
        auto& healthComponent = unit.GetComponent<HealthComponent>();
        healthComponent.amount -= damage;
        projectile.Destroy();
        if (healthComponent.amount <= 0) {
            unit.Destroy();
        }
    }
};
