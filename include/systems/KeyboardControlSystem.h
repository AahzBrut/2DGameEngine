#pragma once
#include "components/KeyboardControlComponent.h"
#include "ecs/ECS.h"
#include "events/KeyPressedEvent.h"
#include "event_bus/EventBus.h"


class KeyboardControlSystem : public System {
public:
    explicit KeyboardControlSystem(const Unique<EventBus> &eventBus) {
        RequireComponent<KeyboardControlComponent>();
        RequireComponent<AnimationComponent>();
        RequireComponent<VelocityComponent>();
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    void OnKeyPressed(KeyPressedEvent &event) {
        for (auto controlledEntity: GetSystemEntities()) {
            auto &animation = controlledEntity.GetComponent<AnimationComponent>();
            auto &velocity = controlledEntity.GetComponent<VelocityComponent>();
            const auto &keyboardControl = controlledEntity.GetComponent<KeyboardControlComponent>();
            switch (event.key) {
                case SDLK_w:
                    animation.currentSequence = 0;
                    velocity.velocity = keyboardControl.UpVector;
                    break;
                case SDLK_d:
                    animation.currentSequence = 1;
                    velocity.velocity = keyboardControl.RightVector;
                    break;
                case SDLK_s:
                    animation.currentSequence = 2;
                    velocity.velocity = keyboardControl.DownVector;
                    break;
                case SDLK_a:
                    animation.currentSequence = 3;
                    velocity.velocity = keyboardControl.LeftVector;
                    break;
                default:
                    break;
            }
        }
    }
};
