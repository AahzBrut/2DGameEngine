#pragma once
#include "ecs/ECS.h"
#include "events/KeyPressedEvent.h"
#include "event_bus/EventBus.h"


class KeyboardControlSystem : public System {
public:
    explicit KeyboardControlSystem(const Unique<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent& event) {
        LOG("OnKeyPressed: {}, {}", event.key, std::string(1, event.key));
    }
};
