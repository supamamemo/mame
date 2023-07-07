#pragma once

#include <wrl.h>
#include <memory>
#include <xaudio2.h>

#include "Audio.h"


enum class BGM
{

};

enum class SE
{

};

class AudioManager
{
private:
    AudioManager()  = default;
    ~AudioManager() = default;

public:
    static AudioManager& Instance()
    {
        static AudioManager instance;
        return instance;
    }

    void LoadAudio();                               // 音楽読み込み

    void StopAllBGM();                              // 全BGM停止
    void StopAllSE();                               // 全SE停止
    void StopAllAudio();                            // 全音楽停止

public:
    std::unique_ptr<Audio>& GetBGM(const BGM& bgm); //　BGM取得
    std::unique_ptr<Audio>& GetSE(const SE& se);    //  SE取得

public:
    // XAUDIO2
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2     = nullptr;
    IXAudio2MasteringVoice*          masterVoice = nullptr;

private:
    std::unique_ptr<Audio> bgm_[8] = {};
    std::unique_ptr<Audio> se_[8]  = {};

};

