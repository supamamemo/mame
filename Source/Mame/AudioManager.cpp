#include "AudioManager.h"

void AudioManager::LoadAudio()
{
    bgm_[0] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/akumanokyoku.wav");
    se_[0]  = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/jump.wav");
    se_[1]  = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/coin.wav");
}


std::unique_ptr<Audio>& AudioManager::GetBGM(const BGM& bgmIndex)
{
    return bgm_[static_cast<int>(bgmIndex)];
}


std::unique_ptr<Audio>& AudioManager::GetSE(const SE& seIndex)
{
    return se_[static_cast<int>(seIndex)];
}


void AudioManager::StopAllBGM()
{
    for (std::unique_ptr<Audio>& bgm :bgm_)
    {
        bgm->Stop();
    }
}


void AudioManager::StopAllSE()
{
    for (std::unique_ptr<Audio>& se : se_)
    {
        se->Stop();
    }
}


void AudioManager::StopAllAudio()
{
    for (std::unique_ptr<Audio>& bgm : bgm_)
    {
        bgm->Stop();
    }
    for (std::unique_ptr<Audio>& se : se_)
    {
        se->Stop();
    }
}
