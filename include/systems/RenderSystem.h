#pragma once
#include "components/TransformComponent.h"
#include "ecs/ECS.h"


class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Render(const Unique<SDL_Renderer> &renderer, const SDL_Rect &camera) const {
        struct EntityTuple {
            Entity entity;
            TransformComponent *transform;
            SpriteComponent *sprite;
        };

        List<EntityTuple> entityTuples;
        for (auto &entity: GetSystemEntities()) {
            entityTuples.emplace_back(EntityTuple{
                entity,
                &entity.GetComponent<TransformComponent>(),
                &entity.GetComponent<SpriteComponent>()
            });
        }

        std::ranges::sort(entityTuples, [](const EntityTuple &a, const EntityTuple &b) {
            return a.sprite->zIndex < b.sprite->zIndex;
        });

        for (const auto &tuple: entityTuples) {
            const auto &transform = tuple.transform;
            const auto &sprite = tuple.sprite;
            const auto texture = sprite->sprite.texture.get();

            SDL_Rect dstRect = {
                static_cast<int>(transform->position.x - static_cast<float>(sprite->fixed ? 0 : camera.x)),
                static_cast<int>(transform->position.y - static_cast<float>(sprite->fixed ? 0 : camera.y)),
                static_cast<int>(static_cast<float>(sprite->rect.w) * transform->scale.x),
                static_cast<int>(static_cast<float>(sprite->rect.h) * transform->scale.y),
            };

            SDL_SetTextureColorMod(texture, sprite->color.r, sprite->color.g, sprite->color.b);
            SDL_SetTextureAlphaMod(texture, sprite->color.a);
            SDL_RenderCopyEx(renderer.get(), texture, &sprite->rect, &dstRect, transform->rotation, nullptr,
                             SDL_FLIP_NONE);
        }
    }
};
