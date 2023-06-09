#pragma once

#include <wrl.h>
#include <memory>
#include <xaudio2.h>

#include "Audio.h"


enum class BGM
{
    Title,
    StageTutorial,
    StageSelection,
    StagePlains,
    StagePlains_Back,   // ´Xe[WÌÂ«¹
    StageEx,   
    StageEx_Back,
    StageBoss,
    StageBoss_Back,     // {XXe[WÌÂ«¹

    StageClear,

    BGM_Max,
};

enum class SE
{
    Select,
    SelectStage,

    PL_Walk,
    PL_Dash,
    PL_Run,
    PL_Brake,
    PL_Jump,
    PL_Landing,
    PL_Bounce,
    PL_BounceHit,
    PL_Damaged,
    PL_Death,

    //Tofu_Walk,
    //Tofu_Run,
    Tofu_Find,

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

    void LoadAudio();                               // ¹yÇÝÝ

    // BGMÄ¶
    void PlayBGM(
        const BGM& bgm, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );    

    // SEÄ¶
    void PlaySE(
        const SE& se, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );       

    void StopBGM(const BGM& bgm);                   // BGMâ~
    void StopSE(const SE& se);                      // SEâ~

    void StopPlayerMoveSE();                        // vC[Ìs®SEâ~
    void StopTofuMoveSE();                         // ¤Ìs®SEâ~

    void StopAllBGM();                              // SBGMâ~
    void StopAllSE();                               // SSEâ~
    void StopAllAudio();                            // S¹yâ~

public:
    std::unique_ptr<Audio>& GetBGM(const BGM& bgm); //@BGMæ¾
    std::unique_ptr<Audio>& GetSE(const SE& se);    //  SEæ¾

public:
    // XAUDIO2
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2     = nullptr;
    IXAudio2MasteringVoice*          masterVoice = nullptr;

private:
    std::unique_ptr<Audio> bgm_[static_cast<int>(BGM::BGM_Max)] = {};
    std::unique_ptr<Audio> se_[static_cast<int>(SE::SE_Max)]    = {};

};

