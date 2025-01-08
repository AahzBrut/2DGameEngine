#pragma once
#include "core/Game.h"
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
            const auto offsetX = sprite.rect.w * transform.scale.x * .5f;
            const auto offsetY = sprite.rect.h * transform.scale.y * .5f;
            const auto widthShift = (camera.w >> 1) - offsetX;
            const auto heightShift = (camera.h >> 1) - offsetY;
            if (posX > widthShift && posX < Game::mapWidth - widthShift - offsetX * 2) {
                camera.x = posX - widthShift;
            }
            if (posY > heightShift && posY < Game::mapHeight - heightShift - offsetY * 2) {
                camera.y = posY - heightShift;
            }
            camera.x = camera.x < 0 ? 0 : camera.x > camera.w ? camera.w : camera.x;
            camera.y = camera.y < 0 ? 0 : camera.y > camera.h ? camera.h : camera.y;
        }
    }
};
