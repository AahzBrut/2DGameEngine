#pragma once
#include "ecs/ECS.h"
#include "glm/vec2.hpp"
#include "glm/detail/func_packing_simd.inl"


struct VelocityComponent : Component<VelocityComponent> {
    glm::vec2 velocity;

    explicit VelocityComponent(const glm::vec2 velocity = glm::vec2(0, 0)) : velocity(velocity) {}
};
