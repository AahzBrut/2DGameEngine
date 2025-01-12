#pragma once
#include "audio_manager/AudioManager.h"
#include "ecs/ECS.h"

class SoundPlaySystem : public System {
    AudioManager *audioManager;

public:
    explicit SoundPlaySystem(const Unique<AudioManager> &audioManager)
        : audioManager{audioManager.get()} {
        RequireComponent<SoundSourceComponent>();
    }

    void Update(const SDL_Rect &camera) const {
        for (auto entity: GetSystemEntities()) {
            const auto &soundComp = entity.GetComponent<SoundSourceComponent>();
            const auto soundPosition = soundComp.position -
                                       (glm::vec2{camera.x, camera.y} + glm::vec2{camera.w / 2, camera.h / 2});
            const auto soundDistance = sqrt(soundPosition.x * soundPosition.x + soundPosition.y * soundPosition.y);
            LOG("Distance to sound: {}", soundDistance);
            if (soundDistance <= 500) {
                audioManager->PlaySound(soundComp.soundAssetId, soundDistance);
            }
            entity.Destroy();
        }
    }
};
