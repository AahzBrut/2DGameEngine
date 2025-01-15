#pragma once
#include "asset_manager/AssetManager.h"
#include "components/HealthComponent.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"
#include "ecs/ECS.h"


class HealthBarSystem : public System {
    AssetManager *assetManager;
    SDL_Color healthyColor = {0, 255, 0, 255};
    SDL_Color damagedColor = {192, 192, 0, 255};
    SDL_Color criticalColor = {255, 0, 0, 255};

    [[nodiscard]] SDL_Color GetHealthColor(const float healthPercent) const {
        return healthPercent > .8f ? healthyColor : healthPercent > .3f ? damagedColor : criticalColor;
    }

    void RenderHealthLabel(SDL_Renderer *renderer, const float healthPercent, const glm::vec2 &position,
                           const float width) const {
        const auto surface = TTF_RenderText_Blended(
            assetManager->GetFont("pico-font-5").get(),
            std::format("{}%", static_cast<int>(healthPercent * 100)).c_str(),
            GetHealthColor(healthPercent));

        const auto texture = SDL_CreateTextureFromSurface(renderer, surface);

        const auto offsetX = (width - static_cast<float>(surface->w)) * 0.5f;

        const auto dstRect = SDL_Rect{
            static_cast<int>(position.x + offsetX),
            static_cast<int>(position.y - 7 - static_cast<float>(surface->h)),
            surface->w,
            surface->h
        };
        SDL_FreeSurface(surface);

        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
    }

    void RenderHealthBar(SDL_Renderer *renderer, const float healthPercent, const glm::vec2 position,
                         const float width) const {
        // ReSharper disable once CppUseStructuredBinding
        const auto healthColor = GetHealthColor(healthPercent);
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(renderer, healthColor.r, healthColor.g, healthColor.b, healthColor.a);
        const auto healthBarWidth = static_cast<int>(healthPercent * width);

        const auto dstRect = SDL_Rect{
            static_cast<int>(position.x) + healthBarWidth / 10,
            static_cast<int>(position.y) - 5,
            healthBarWidth * 8 / 10,
            5
        };

        SDL_RenderFillRect(renderer, &dstRect);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }

public:
    explicit HealthBarSystem(const Unique<AssetManager> &assetManager)
        : assetManager{assetManager.get()} {
        RequireComponent<HealthComponent>();
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void Render(const Unique<SDL_Renderer> &renderer, const SDL_Rect &camera) {
        const auto pico5Font = assetManager->GetFont("pico-5-font");
        const auto pico10Font = assetManager->GetFont("pico-5-font");
        for (const auto &entity: GetSystemEntities()) {
            const auto &healthComponent = entity.GetComponent<HealthComponent>();
            const auto &transformComponent = entity.GetComponent<TransformComponent>();
            const auto &spriteComponent = entity.GetComponent<SpriteComponent>();
            const auto healthPercent = healthComponent.GetPercentage();
            const auto labelPosition = glm::vec2(transformComponent.position.x - static_cast<float>(camera.x),
                                                 transformComponent.position.y - static_cast<float>(camera.y));

            RenderHealthLabel(renderer.get(), healthPercent, labelPosition,
                              static_cast<float>(spriteComponent.rect.w) * transformComponent.scale.x);
            RenderHealthBar(renderer.get(), healthPercent, labelPosition,
                            static_cast<float>(spriteComponent.rect.w) * transformComponent.scale.x);
        }
    }
};
