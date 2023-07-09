#include "AudioManager.h"

void AudioManager::LoadAudio()
{
    // âπäyì«Ç›çûÇ›
    {
        // BGMì«Ç›çûÇ›
        {
            bgm_[static_cast<int>(BGM::Title)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Title.wav");
            bgm_[static_cast<int>(BGM::Tutorial) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Tutorial.wav");
            bgm_[static_cast<int>(BGM::Stage)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Stage.wav");
            bgm_[static_cast<int>(BGM::Boss)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Boss.wav");
            bgm_[static_cast<int>(BGM::Boss_Back)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/BossBackSound.wav");
        }

        // SEì«Ç›çûÇ›
        {
            // Player
            se_[static_cast<int>(SE::PL_Walk)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerWalk.wav");
            se_[static_cast<int>(SE::PL_Dash)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerDash.wav");
            se_[static_cast<int>(SE::PL_Run)      ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerRun.wav");
            se_[static_cast<int>(SE::PL_Brake)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerBrake.wav");
            se_[static_cast<int>(SE::PL_Jump)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerJump.wav");
            se_[static_cast<int>(SE::PL_Landing)  ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerLanding.wav");
            se_[static_cast<int>(SE::PL_Bounce)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerBounce1.wav");
            se_[static_cast<int>(SE::PL_BounceHit)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerBounceHit.wav");
            se_[static_cast<int>(SE::PL_Damaged)  ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Player/PlayerDamaged.wav");

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
            bgm_[static_cast<int>(BGM::Boss)      ]->Volume(0.65f);
            bgm_[static_cast<int>(BGM::Boss_Back) ]->Volume(0.25f);
        }

        // SEâπó í≤êÆ
        {
            // Player
            se_[static_cast<int>(SE::PL_Walk)     ]->Volume(0.5f);
            se_[static_cast<int>(SE::PL_Dash)     ]->Volume(0.75f);
            se_[static_cast<int>(SE::PL_Run)      ]->Volume(0.75f);
            se_[static_cast<int>(SE::PL_Brake)    ]->Volume(0.5f);
            se_[static_cast<int>(SE::PL_Jump)     ]->Volume(0.75f);
            se_[static_cast<int>(SE::PL_Landing)  ]->Volume(0.5f);
            se_[static_cast<int>(SE::PL_Bounce)   ]->Volume(0.5f);
            se_[static_cast<int>(SE::PL_BounceHit)]->Volume(1.0f);
            se_[static_cast<int>(SE::PL_Damaged)  ]->Volume(2.0f);

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
    se_[static_cast<int>(SE::PL_Walk)]->Stop();
    //se_[static_cast<int>(SE::PL_Dash)]->Stop();
    se_[static_cast<int>(SE::PL_Run)]->Stop();
    se_[static_cast<int>(SE::PL_Brake)]->Stop();
    se_[static_cast<int>(SE::PL_Jump)]->Stop();
    //se_[static_cast<int>(SE::PL_Landing)]->Stop();
    //se_[static_cast<int>(SE::PL_Bounce)]->Stop();
    //se_[static_cast<int>(SE::PL_BounceHit)]->Stop();
    //se_[static_cast<int>(SE::PL_Damaged)]->Stop();
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
