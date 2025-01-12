#include "audio_manager/AudioManager.h"


void AudioManager::Initialize() {
    Mix_OpenAudio(frequency, format, audioChannels, bufferSize);
    Mix_AllocateChannels(MaxChannelsCount);
}

void AudioManager::Shutdown() {
    Mix_CloseAudio();
    Mix_Quit();
}

void AudioManager::PlayMusic(const std::string &assetId, const int loopCount) {
    Mix_PlayMusic(assetManager->GetMusic(assetId).get(), loopCount);
}

int AudioManager::PlaySound(const std::string &assetId) {
    return Mix_PlayChannel(-1, assetManager->GetSoundEffect(assetId).get(), 0);
}
