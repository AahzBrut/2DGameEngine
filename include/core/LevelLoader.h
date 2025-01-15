#pragma once
#include <sol/state.hpp>

#include "SmartPointers.h"
#include "asset_manager/AssetManager.h"
#include "ecs/ECS.h"

class LevelLoader {
    sol::state &lua;
    Unique<AssetManager> &assetManager;
    Unique<SDL_Renderer> &renderer;
    Unique<Registry> &registry;

public:
    LevelLoader(sol::state &lua, Unique<AssetManager> &assetManager, Unique<SDL_Renderer> &renderer,
                Unique<Registry> &registry);
    ~LevelLoader() = default;

    void LoadLevel(int levelId) const;

private:
    static List<List<SDL_Rect> > GetAnimationFrames(const sol::table &sourceTable);
    static void LoadTransformComponent(Entity entity, const sol::table &transformDef);
    void LoadSpriteComponent(Entity entity, const sol::table &spriteDef) const;
};
