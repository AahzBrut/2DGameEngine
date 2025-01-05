#pragma once
#include <glm/vec2.hpp>

#include "ecs/ECS.h"


struct TransformComponent : Component<TransformComponent> {
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;
};
