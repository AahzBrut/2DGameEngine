#pragma once
#include "audio_manager/AudioManager.h"
#include "ecs/ECS.h"

class SoundPlaySystem : public System {
    AudioManager *audioManager;
    EventBus *eventBus;
    List<SoundEmitterEvent> events;

public:
    explicit SoundPlaySystem(
        const Unique<AudioManager> &audioManager,
        const Unique<EventBus> &eventBus)
        : audioManager{audioManager.get()},
          eventBus{eventBus.get()} {
        RequireComponent<TransformComponent>();
        RequireComponent<CameraFollowComponent>();

        eventBus->SubscribeToEvent(this, OnSoundEmitted);
    }

    void Update() {
        const auto listeners = GetSystemEntities();
        if (listeners.empty()) return;

        const auto &listenerTransform = listeners.front().GetComponent<TransformComponent>();

        for (const auto& event: events) {
            const auto soundPosition = listenerTransform.position - event.position;
            const auto soundDistance = sqrt(soundPosition.x * soundPosition.x + soundPosition.y * soundPosition.y);
            LOG("Distance to sound: {}", soundDistance);
            if (soundDistance <= 500) {
                audioManager->PlaySound(event.soundAssetId, soundDistance);
            }
        }
        events.clear();
    }

    void OnSoundEmitted(SoundEmitterEvent &event) {
        events.emplace_back(event);
    }
};
