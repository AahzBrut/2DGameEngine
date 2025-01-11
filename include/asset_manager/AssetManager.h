#pragma once
#include <string>

#include <SDL_mixer.h>
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

struct SoundEffectDeleter {
    void operator()(Mix_Chunk *pointer) const {
        if (pointer) {
            Mix_FreeChunk(pointer);
            LOG("Destroying sound effect");
        }
    }
};

struct MusicDeleter {
    void operator()(Mix_Music *pointer) const {
        if (pointer) {
            Mix_FreeMusic(pointer);
            LOG("Destroying music truck");
        }
    }
};

class AssetManager {
    Dictionary<std::string, Sprite> textures;
    Dictionary<std::string, Shared<Mix_Chunk>> soundEffects;
    Dictionary<std::string, Shared<Mix_Music>> musicTrucks;

public:
    AssetManager();
    ~AssetManager();

    void Clear();
    void ClearUnused();

    void LoadTexture(const Unique<SDL_Renderer> &renderer, const std::string &assetId, const std::string &path);
    void LoadSoundEffect(const std::string &assetId, const std::string &path);
    void LoadMusic(const std::string &assetId, const std::string &path);

    Shared<Mix_Chunk> GetSoundEffect(const std::string &assetId);
    Shared<Mix_Music> GetMusic(const std::string &assetId);
    Sprite &GetTexture(const std::string &assetId);
};
