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

    void LoadAudio();                               // ���y�ǂݍ���

    void StopAllBGM();                              // �SBGM��~
    void StopAllSE();                               // �SSE��~
    void StopAllAudio();                            // �S���y��~

public:
    std::unique_ptr<Audio>& GetBGM(const BGM& bgm); //�@BGM�擾
    std::unique_ptr<Audio>& GetSE(const SE& se);    //  SE�擾

public:
    // XAUDIO2
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2     = nullptr;
    IXAudio2MasteringVoice*          masterVoice = nullptr;

private:
    std::unique_ptr<Audio> bgm_[8] = {};
    std::unique_ptr<Audio> se_[8]  = {};

};

