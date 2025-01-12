#pragma once
#include <string>
#include <utility>

#include "glm/vec2.hpp"


struct SoundSourceComponent {
    glm::vec2 position;
    std::string soundAssetId;

    explicit SoundSourceComponent(const glm::vec2 &position = glm::vec2{0}, std::string sound_asset_id = "")
        : position{position},
          soundAssetId{std::move(sound_asset_id)} {}
};
