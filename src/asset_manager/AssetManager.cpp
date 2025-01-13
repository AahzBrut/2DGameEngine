#include "asset_manager/AssetManager.h"

#include <SDL_image.h>
#include "core/Collections.h"


AssetManager::AssetManager() {
    LOG("AssetManager created.");
}

AssetManager::~AssetManager() {
    textures.clear();
    soundEffects.clear();
    musicTrucks.clear();
    fonts.clear();
    LOG("AssetManager destroyed.");
}

void AssetManager::Clear() {
    textures.clear();
    soundEffects.clear();
    musicTrucks.clear();
    fonts.clear();
}

void AssetManager::ClearUnused() {
    for (auto it = textures.begin(); it != textures.end();) {
        if (it->second.texture.unique()) {
            it = textures.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = soundEffects.begin(); it != soundEffects.end();) {
        if (it->second.unique()) {
            it = soundEffects.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = musicTrucks.begin(); it != musicTrucks.end();) {
        if (it->second.unique()) {
            it = musicTrucks.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = fonts.begin(); it != fonts.end();) {
        if (it->second.unique()) {
            it = fonts.erase(it);
        } else {
            ++it;
        }
    }
}

void AssetManager::LoadTexture(const Unique<SDL_Renderer> &renderer, const std::string &assetId,
                               const std::string &path) {
    const auto surface = IMG_Load(path.c_str());
    const auto texture = Shared<SDL_Texture>(SDL_CreateTextureFromSurface(renderer.get(), surface), SDLDeleter<SDL_Texture>{});
    textures.emplace(assetId, Sprite{surface->w, surface->h, texture});
    SDL_FreeSurface(surface);
}

void AssetManager::LoadSoundEffect(const std::string &assetId, const std::string &path) {
    const auto soundEffect = Shared<Mix_Chunk>(Mix_LoadWAV(path.c_str()), SDLDeleter<Mix_Chunk>{});
    soundEffects.emplace(assetId, soundEffect);
}

void AssetManager::LoadMusic(const std::string &assetId, const std::string &path) {
    const auto musicTruck = Shared<Mix_Music>(Mix_LoadMUS(path.c_str()), SDLDeleter<Mix_Music>{});
    musicTrucks.emplace(assetId, musicTruck);
}

void AssetManager::LoadFont(const std::string &assetId, const std::string &path, const int fontSize) {
    const auto font = TTF_OpenFont(path.c_str(), fontSize);
    fonts.emplace(assetId, Shared<TTF_Font>(font, SDLDeleter<TTF_Font>{}));
}

Shared<Mix_Chunk> AssetManager::GetSoundEffect(const std::string &assetId) {
    return soundEffects[assetId];
}

Shared<Mix_Music> AssetManager::GetMusic(const std::string &assetId) {
    return musicTrucks[assetId];
}

Sprite &AssetManager::GetTexture(const std::string &assetId) {
    return textures[assetId];
}

Shared<TTF_Font> AssetManager::GetFont(const std::string &assetId) {
    return fonts[assetId];
}
