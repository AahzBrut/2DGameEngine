#pragma once
#include <string>

#include "asset_manager/AssetManager.h"

constexpr auto MaxChannelsCount = 32;

class AudioManager {
    AssetManager *assetManager;

    int frequency;
    Uint16 format;
    int audioChannels;
    int bufferSize;

public:
    explicit AudioManager(
        const Unique<AssetManager> &assetManager,
        const int frequency = MIX_DEFAULT_FREQUENCY,
        const Uint16 format = MIX_DEFAULT_FORMAT,
        const int audioChannels = MIX_DEFAULT_CHANNELS,
        const int bufferSize = 2048)
        : assetManager{assetManager.get()},
          frequency{frequency},
          format{format},
          audioChannels{audioChannels},
          bufferSize{bufferSize} {}

    void Initialize() const;
    void Shutdown();

    void PlayMusic(const std::string &assetId, int loopCount) const;
    void PlaySound(const std::string &assetId) const;
    void PlaySound(const std::string &assetId, float distance) const;
};
