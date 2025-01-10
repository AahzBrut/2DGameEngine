#pragma once
#include "glm/vec2.hpp"


struct DirectionComponent {
    glm::vec2 direction;

    explicit DirectionComponent(
        const glm::vec2 &direction = glm::vec2{1, 0})
    : direction(direction) {}
};
