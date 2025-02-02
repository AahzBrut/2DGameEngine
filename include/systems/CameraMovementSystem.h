#pragma once
#include "components/CameraFollowComponent.h"
#include "components/TransformComponent.h"
#include "ecs/ECS.h"


class CameraMovementSystem : public System {
public:
    CameraMovementSystem() {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Rect &camera) const {
        for (auto entity: GetSystemEntities()) {
            const auto &transform = entity.GetComponent<TransformComponent>();
            const auto &sprite = entity.GetComponent<SpriteComponent>();

            const auto posX = transform.position.x;
            const auto posY = transform.position.y;
            const auto offsetX = static_cast<float>(sprite.rect.w) * transform.scale.x * .5f;
            const auto offsetY = static_cast<float>(sprite.rect.h) * transform.scale.y * .5f;
            const auto widthShift = static_cast<float>(camera.w >> 1) - offsetX;
            const auto heightShift = static_cast<float>(camera.h >> 1) - offsetY;
            if (posX > widthShift && posX < static_cast<float>(Game::mapWidth) - widthShift - offsetX * 2) {
                camera.x = static_cast<int>(posX - widthShift);
            }
            if (posY > heightShift && posY < static_cast<float>(Game::mapHeight) - heightShift - offsetY * 2) {
                camera.y = static_cast<int>(posY - heightShift);
            }
            camera.x = camera.x < 0 ? 0 : camera.x > camera.w ? camera.w : camera.x;
            camera.y = camera.y < 0 ? 0 : camera.y > camera.h ? camera.h : camera.y;
        }
    }
};
