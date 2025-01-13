#include "audio_manager/AudioManager.h"


void AudioManager::Initialize() const {
    Mix_OpenAudio(frequency, format, audioChannels, bufferSize);
    Mix_AllocateChannels(MaxChannelsCount);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void AudioManager::Shutdown() { // NOLINT(*-convert-member-functions-to-static)
    Mix_CloseAudio();
    Mix_Quit();
}

void AudioManager::PlayMusic(const std::string &assetId, const int loopCount) const {
    Mix_PlayMusic(assetManager->GetMusic(assetId).get(), loopCount);
}

void AudioManager::PlaySound(const std::string &assetId) const {
    Mix_PlayChannel(-1, assetManager->GetSoundEffect(assetId).get(), 0);
}

void AudioManager::PlaySound(const std::string &assetId, const float distance) const {
    const auto channel = Mix_PlayChannel(-1, assetManager->GetSoundEffect(assetId).get(), 0);
    Mix_SetDistance(channel, static_cast<Uint8>(distance * .5f));
}
