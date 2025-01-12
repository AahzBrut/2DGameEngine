#pragma once
#include <string>

#include "event_bus/Event.h"
#include "glm/vec2.hpp"


class SoundEmitterEvent : public  Event {
    public:
    glm::vec2 position;
    std::string soundAssetId;

    explicit SoundEmitterEvent(const glm::vec2 &position = glm::vec2{0}, std::string sound_asset_id = "")
        : position{position},
          soundAssetId{std::move(sound_asset_id)} {}
};
