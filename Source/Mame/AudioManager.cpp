#include "AudioManager.h"

void AudioManager::LoadAudio()
{
    // âπäyì«Ç›çûÇ›
    {
        // BGMì«Ç›çûÇ›
        {
            bgm_[static_cast<int>(BGM::Title)           ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Title.wav");
            bgm_[static_cast<int>(BGM::StageTutorial)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/StageTutorial.wav");
            bgm_[static_cast<int>(BGM::StageSelection)  ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/StageSelection.wav");
            bgm_[static_cast<int>(BGM::StagePlains)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/StagePlains.wav");
            bgm_[static_cast<int>(BGM::StagePlains_Back)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/StagePlainsBackSound.wav");
            bgm_[static_cast<int>(BGM::StageEx)         ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/StageEx.wav");
            bgm_[static_cast<int>(BGM::StageEx_Back)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/StageExBackSound.wav");
            bgm_[static_cast<int>(BGM::StageBoss)       ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/StageBoss.wav");
            bgm_[static_cast<int>(BGM::StageBoss_Back)  ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/StageBossBackSound.wav");
            bgm_[static_cast<int>(BGM::StageClear)      ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/Jingle/StageClear.wav");
        }

        // SEì«Ç›çûÇ›
        {
            // Select
            se_[static_cast<int>(SE::Select)      ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Select/Select.wav");
            se_[static_cast<int>(SE::SelectStage) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Select/SelectStage.wav");
            
            // Player
            se_[static_cast<int>(SE::PL_Walk)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerWalk.wav");
            se_[static_cast<int>(SE::PL_Dash)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerDash.wav");
            se_[static_cast<int>(SE::PL_Run)      ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerRun.wav");
            se_[static_cast<int>(SE::PL_Brake)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerBrake.wav");
            se_[static_cast<int>(SE::PL_Jump)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerJump.wav");
            se_[static_cast<int>(SE::PL_Landing)  ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerLanding.wav");
            se_[static_cast<int>(SE::PL_Bounce)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerBounce.wav");
            se_[static_cast<int>(SE::PL_BounceHit)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerBounceHit.wav");
            se_[static_cast<int>(SE::PL_Damaged)  ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerDamaged.wav");
            se_[static_cast<int>(SE::PL_Death)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerDeath.wav");
            
            // Tofu
            //se_[static_cast<int>(SE::Tofu_Walk)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Tofu/TofuWalk.wav");
            //se_[static_cast<int>(SE::Tofu_Run)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Tofu/TofuRun.wav");
            se_[static_cast<int>(SE::Tofu_Find)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Tofu/TofuFind.wav");

            // Boss
            se_[static_cast<int>(SE::Boss_Walk)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Boss/BossWalk.wav");
            se_[static_cast<int>(SE::Boss_Run)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Boss/BossRun.wav");
            se_[static_cast<int>(SE::Boss_Turn)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Boss/BossTurn.wav");
            se_[static_cast<int>(SE::Boss_HitWall)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Boss/BossHitWall.wav");
            se_[static_cast<int>(SE::Boss_Stun)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Boss/BossStun.wav");
            se_[static_cast<int>(SE::Boss_Down)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Boss/BossDown.wav");
            se_[static_cast<int>(SE::Boss_Cry)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Boss/BossCry.wav");
        }
    }

    // âπó í≤êÆ
    {    
        // BGMâπó í≤êÆ
        {
            bgm_[static_cast<int>(BGM::Title)           ]->Volume(0.75f);
            bgm_[static_cast<int>(BGM::StagePlains_Back)]->Volume(0.5f);
            bgm_[static_cast<int>(BGM::StageEx_Back)    ]->Volume(0.25f);
            bgm_[static_cast<int>(BGM::StageBoss)       ]->Volume(0.65f);
            bgm_[static_cast<int>(BGM::StageBoss_Back)  ]->Volume(0.25f);
        }

        // SEâπó í≤êÆ
        {
            // Player
            se_[static_cast<int>(SE::PL_Walk)     ]->Volume(0.6f);
            se_[static_cast<int>(SE::PL_Dash)     ]->Volume(0.75f);
            se_[static_cast<int>(SE::PL_Run)      ]->Volume(0.75f);
            se_[static_cast<int>(SE::PL_Brake)    ]->Volume(0.5f);
            se_[static_cast<int>(SE::PL_Jump)     ]->Volume(0.75f);
            se_[static_cast<int>(SE::PL_Landing)  ]->Volume(0.5f);
            se_[static_cast<int>(SE::PL_Bounce)   ]->Volume(0.5f);
            se_[static_cast<int>(SE::PL_BounceHit)]->Volume(1.0f);
            se_[static_cast<int>(SE::PL_Damaged)  ]->Volume(2.0f);
            se_[static_cast<int>(SE::PL_Death)    ]->Volume(0.75f);

            // Tofu
            //se_[static_cast<int>(SE::Tofu_Walk)   ]->Volume(0.75f);
            //se_[static_cast<int>(SE::Tofu_Run)    ]->Volume(0.75f);
            se_[static_cast<int>(SE::Tofu_Find)    ]->Volume(0.5f);

            // Boss
            se_[static_cast<int>(SE::Boss_Walk)   ]->Volume(2.5f);
            se_[static_cast<int>(SE::Boss_Run)    ]->Volume(3.5f);
            se_[static_cast<int>(SE::Boss_Turn)   ]->Volume(0.5f);
            se_[static_cast<int>(SE::Boss_HitWall)]->Volume(2.0f);
        }
    }

}


void AudioManager::PlayBGM(const BGM& bgm, const bool isLoop, const bool isIgnoreQueue)
{
    bgm_[static_cast<int>(bgm)]->Play(isLoop, isIgnoreQueue);
}

void AudioManager::PlaySE(const SE& se, const bool isLoop, const bool isIgnoreQueue)
{
    se_[static_cast<int>(se)]->Play(isLoop, isIgnoreQueue);
}


void AudioManager::StopBGM(const BGM& bgm)
{
    bgm_[static_cast<int>(bgm)]->Stop();
}

void AudioManager::StopSE(const SE& se)
{
    se_[static_cast<int>(se)]->Stop();
}

void AudioManager::StopPlayerMoveSE()
{
    se_[static_cast<int>(SE::PL_Walk) ]->Stop();
    se_[static_cast<int>(SE::PL_Run)  ]->Stop();
    se_[static_cast<int>(SE::PL_Brake)]->Stop();
    se_[static_cast<int>(SE::PL_Jump) ]->Stop();
}

void AudioManager::StopTofuMoveSE()
{
    //se_[static_cast<int>(SE::Tofu_Walk)]->Stop();
    //se_[static_cast<int>(SE::Tofu_Run) ]->Stop();
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
