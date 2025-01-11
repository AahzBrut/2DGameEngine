#include "asset_manager/AssetManager.h"

#include <SDL_image.h>
#include "core/Collections.h"


AssetManager::AssetManager() {
    LOG("AssetManager created.");
}

AssetManager::~AssetManager() {
    textures.clear();
    LOG("AssetManager destroyed.");
}

void AssetManager::Clear() {
    textures.clear();
}

void AssetManager::ClearUnused() {
    for (auto it = textures.begin(); it != textures.end();) {
        if (it->second.texture.unique()) {
            it = textures.erase(it);
        } else {
            ++it;
        }
    }
}

void AssetManager::LoadTexture(const Unique<SDL_Renderer> &renderer, const std::string &assetId,
                               const std::string &path) {
    const auto surface = IMG_Load(path.c_str());
    const auto texture = Shared<SDL_Texture>(SDL_CreateTextureFromSurface(renderer.get(), surface), TextureDeleter{});
    textures.emplace(assetId, Sprite{surface->w, surface->h, texture});
    SDL_FreeSurface(surface);
}

void AssetManager::LoadSoundEffect(const std::string &assetId, const std::string &path) {
    const auto soundEffect = Shared<Mix_Chunk>(Mix_LoadWAV(path.c_str()), SoundEffectDeleter{});
    soundEffects.emplace(assetId, soundEffect);
}

void AssetManager::LoadMusic(const std::string &assetId, const std::string &path) {
    const auto musicTruck = Shared<Mix_Music>(Mix_LoadMUS(path.c_str()), MusicDeleter{});
    musicTrucks.emplace(assetId, musicTruck);
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
