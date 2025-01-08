#pragma once
#include "ecs/ECS.h"
#include "event_bus/Event.h"

class CollisionEvent: public Event {
public:
    Entity firstEntity;
    Entity secondEntity;

    CollisionEvent(const Entity &firstEntity, const Entity &secondEntity)
        : firstEntity{firstEntity}, secondEntity{secondEntity} {}
};
