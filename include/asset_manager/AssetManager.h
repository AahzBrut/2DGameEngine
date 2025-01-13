#pragma once
#include <string>

#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_render.h>

#include "core/Collections.h"
#include "core/SmartPointers.h"
#include "data/Sprite.h"

template<typename SDLType>
struct SDLDeleter {
    void operator()(SDLType *pointer) const {
        if (pointer) {
            if constexpr (std::is_same_v<SDLType, SDL_Texture>) {
                SDL_DestroyTexture(pointer);
                LOG("Destroying texture");
            } else if constexpr (std::is_same_v<SDLType, Mix_Chunk>) {
                Mix_FreeChunk(pointer);
                LOG("Destroying sound effect");
            } else if constexpr (std::is_same_v<SDLType, Mix_Music>) {
                Mix_FreeMusic(pointer);
                LOG("Destroying music truck");
            } else if constexpr (std::is_same_v<SDLType, TTF_Font>) {
                TTF_CloseFont(pointer);
                LOG("Destroying font");
            }
        }
    }
};

class AssetManager {
    Dictionary<std::string, Sprite> textures;
    Dictionary<std::string, Shared<Mix_Chunk>> soundEffects;
    Dictionary<std::string, Shared<Mix_Music>> musicTrucks;
    Dictionary<std::string, Shared<TTF_Font>> fonts;

public:
    AssetManager();
    ~AssetManager();

    void Clear();
    void ClearUnused();

    void LoadTexture(const Unique<SDL_Renderer> &renderer, const std::string &assetId, const std::string &path);
    void LoadSoundEffect(const std::string &assetId, const std::string &path);
    void LoadMusic(const std::string &assetId, const std::string &path);
    void LoadFont(const std::string &assetId, const std::string &path, int fontSize);

    Shared<Mix_Chunk> GetSoundEffect(const std::string &assetId);
    Shared<Mix_Music> GetMusic(const std::string &assetId);
    Sprite &GetTexture(const std::string &assetId);
    Shared<TTF_Font> GetFont(const std::string &assetId);
};
