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
    Boss_Back, // ボス戦の環境音

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

    void LoadAudio();                               // 音楽読み込み

    // BGM再生
    void PlayBGM(
        const BGM& bgm, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );    

    // SE再生
    void PlaySE(
        const SE& se, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );       

    void StopBGM(const BGM& bgm);                   // BGM停止
    void StopSE(const SE& se);                      // SE停止

    void StopPlayerMoveSE();                     // プレイヤーの行動SE停止

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
    std::unique_ptr<Audio> bgm_[static_cast<int>(BGM::BGM_Max)] = {};
    std::unique_ptr<Audio> se_[static_cast<int>(SE::SE_Max)]    = {};

};

