#pragma once
#include "components/AnimationComponent.h"
#include "components/SpriteComponent.h"
#include "ecs/ECS.h"


class AnimationSystem : public System {
public:
    AnimationSystem() {
        RequireComponent<AnimationComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update([[maybe_unused]]const float deltaTime) const {
        for (const auto &entity: GetSystemEntities()) {
            auto &animation = entity.GetComponent<AnimationComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();

            const auto currentTicks = SDL_GetTicks();
            const auto framesPassed = static_cast<int>((currentTicks - animation.startTime) / animation.delayBetweenFrames);
            const auto frameCount = animation.frames[animation.currentSequence].size();
            animation.currentFrame = framesPassed % static_cast<int>(frameCount);

            sprite.rect = animation.frames[animation.currentSequence][animation.currentFrame];
        }
    }
};
