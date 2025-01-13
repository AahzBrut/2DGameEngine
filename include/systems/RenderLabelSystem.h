#pragma once
#include "ecs/ECS.h"


class RenderLabelSystem : public System {
public:
    RenderLabelSystem() {
        RequireComponent<LabelComponent>();
    }

    void Render(const Unique<SDL_Renderer> &renderer) const {
        for (auto entity: GetSystemEntities()) {
            const auto &labelComponent = entity.GetComponent<LabelComponent>();
            const auto surface = TTF_RenderText_Blended(
                labelComponent.font.get(),
                labelComponent.text.c_str(),
                labelComponent.color);

            const auto texture = SDL_CreateTextureFromSurface(renderer.get(), surface);

            const auto dstRect = SDL_Rect{
                static_cast<int>(labelComponent.position.x),
                static_cast<int>(labelComponent.position.y),
                surface->w,
                surface->h
            };
            SDL_FreeSurface(surface);

            SDL_RenderCopy(renderer.get(), texture, nullptr, &dstRect);
            SDL_DestroyTexture(texture);
        }
    }
};
