#pragma once
#include <string>

#include <SDL2/SDL_render.h>
#include "core/Collections.h"
#include "core/SmartPointers.h"
#include "data/Sprite.h"

struct TextureDeleter {
    void operator()(SDL_Texture *pointer) const {
        if (pointer) {
            SDL_DestroyTexture(pointer);
            LOG("Destroying texture");
        }
    }
};

class AssetManager {
    Dictionary<std::string, Sprite> textures;

public:
    AssetManager();
    ~AssetManager();

    void Clear();
    void ClearUnused();

    void LoadTexture(const Unique<SDL_Renderer>& renderer, const std::string &assetId, const std::string &path);
    Sprite& GetTexture(const std::string &assetId);
};
