#pragma once
#include "glm/vec2.hpp"

struct KeyboardControlComponent {
    typedef glm::vec2 Vec2;
    Vec2 UpVector;
    Vec2 RightVector;
    Vec2 DownVector;
    Vec2 LeftVector;

    explicit KeyboardControlComponent(
        const Vec2 &up_vector = Vec2{0},
        const Vec2 &right_vector = Vec2{0},
        const Vec2 &down_vector = Vec2{0},
        const Vec2 &left_vector = Vec2{0})
        : UpVector{up_vector},
          RightVector{right_vector},
          DownVector{down_vector},
          LeftVector{left_vector} {}
};
