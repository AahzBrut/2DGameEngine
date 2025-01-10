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
        RequireComponent<ProjectileEmitterComponent>();
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    void OnKeyPressed(KeyPressedEvent &event) {
        for (auto controlledEntity: GetSystemEntities()) {
            auto &animation = controlledEntity.GetComponent<AnimationComponent>();
            auto &velocity = controlledEntity.GetComponent<VelocityComponent>();
            auto &direction = controlledEntity.GetComponent<DirectionComponent>();
            auto &emitter = controlledEntity.GetComponent<ProjectileEmitterComponent>();
            const auto &keyboardControl = controlledEntity.GetComponent<KeyboardControlComponent>();
            switch (event.key) {
                case SDLK_w:
                    animation.currentSequence = 0;
                    if (event.eventType == SDL_KEYDOWN) {
                        direction.direction = glm::vec2{0, -1};
                        velocity.velocity = keyboardControl.UpVector;
                    } else {
                        velocity.velocity = glm::vec2{0};
                    }
                    break;
                case SDLK_d:
                    animation.currentSequence = 1;
                    if (event.eventType == SDL_KEYDOWN) {
                        direction.direction = glm::vec2{1, 0};
                        velocity.velocity = keyboardControl.RightVector;
                    } else {
                        velocity.velocity = glm::vec2{0};
                    }
                    break;
                case SDLK_s:
                    animation.currentSequence = 2;
                    if (event.eventType == SDL_KEYDOWN) {
                        direction.direction = glm::vec2{0, 1};
                        velocity.velocity = keyboardControl.DownVector;
                    } else {
                        velocity.velocity = glm::vec2{0};
                    }
                    break;
                case SDLK_a:
                    animation.currentSequence = 3;
                    if (event.eventType == SDL_KEYDOWN) {
                        direction.direction = glm::vec2{-1, 0};
                        velocity.velocity = keyboardControl.LeftVector;
                    } else {
                        velocity.velocity = glm::vec2{0};
                    }
                    break;
                case SDLK_SPACE:
                    if (event.eventType == SDL_KEYDOWN) {
                        emitter.autoShoot = true;
                    } else {
                        emitter.autoShoot = false;
                    }
                break;
                default:
                    break;
            }
        }
    }
};
