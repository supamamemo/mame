#pragma once

#include <wrl.h>
#include <memory>
#include <xaudio2.h>

#include "Audio.h"


enum class BGM
{
    Title,
    Tutorial,
    Stage,
    Boss,
    Boss_Back, // �{�X��̊���

    BGM_Max,
};

enum class SE
{
    PL_Walk,
    PL_Dash,
    PL_Run,
    PL_Brake,
    PL_Jump,
    PL_Landing,
    PL_Bounce,
    PL_BounceHit,
    PL_Damaged,

    Boss_Walk,
    Boss_Run,
    Boss_Turn,
    Boss_HitWall,
    Boss_Stun,
    Boss_Down,
    Boss_Cry,

    SE_Max,
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

    // BGM�Đ�
    void PlayBGM(
        const BGM& bgm, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );    

    // SE�Đ�
    void PlaySE(
        const SE& se, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );       

    void StopBGM(const BGM& bgm);                   // BGM��~
    void StopSE(const SE& se);                      // SE��~

    void StopPlayerMoveSE();                     // �v���C���[�̍s��SE��~

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
    std::unique_ptr<Audio> bgm_[static_cast<int>(BGM::BGM_Max)] = {};
    std::unique_ptr<Audio> se_[static_cast<int>(SE::SE_Max)]    = {};

};

