#include "StageEx.h"

#include "../../Mame/Graphics/Graphics.h"
#include "../../Mame/Graphics/Camera.h"
#include "../../Mame/Scene/SceneManager.h"
#include "../../Mame/AudioManager.h"

#include "../Terrain/TerrainNormal.h"
#include "../Terrain/TerrainFall.h"
#include "../Terrain/TerrainDelayFall.h"
#include "../Terrain/TerrainNoClimb.h"

#include "../EnemyTofu.h"
#include "../EnemyRedTofu.h"

#include "../UIManager.h"

#include "StageManager.h"
#include "StageLoading.h"
#include "StageSelection.h"

StageEx::StageEx()
{
}

void StageEx::Initialize()
{
    StageManager& stageManager = StageManager::Instance();

    // ステージの属性を設定
    SetStageType(static_cast<int>(Mame::Stage::TYPE::Ex));

    // 生成
    {
        // terrain生成
        Terrain::nameNum = 0;
        TerrainManager& terrainManager = TerrainManager::Instance();
        RegisterTerrains(terrainManager);

        // player生成
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

        // enemy生成
        EnemyTofu::nameNum = 0;
        EnemyManager& enemyManager = EnemyManager::Instance();
        RegisterEnemies(enemyManager);

        Box::nameNum = 0;
        back = std::make_unique<Box>("./resources/tutorialBack.fbx");

        // UI
        {
            UIManager& uiManager = UIManager::Instance();
            UI::nameNum = 0;
            uiManager.Register(new UI(L"./resources/ui/baseMameHp.png"));
            uiManager.Register(new UI(L"./resources/ui/mameLeft.png"));
            uiManager.Register(new UI(L"./resources/ui/mameCenter.png"));
            uiManager.Register(new UI(L"./resources/ui/mameRight.png"));
        }
    }

    // camera初期化
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 8.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0, 0, 0));

    // terrain初期化
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        SetTerrains(terrainManager);
        terrainManager.Initialize();
    }

    // player初期化
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        // 保存してる中間地点がここのステージの中間地点だったら中間地点の位置をプレイヤーに代入
        if (stageManager.savedHalfPoint_.savedHalfwayPointStage == StageManager::SavedHalfwayPointStage::StagePlains)
        {
            playerManager.GetPlayer()->GetTransform()->SetPosition(stageManager.savedHalfPoint_.position);
            playerManager.GetPlayer()->GetTransform()->SetPositionZ(10.0f);
        }
        // 保存してる中間地点がここのステージの中間地点でなければプレイヤーに初期位置を代入し、保存した中間地点情報をリセット
        else
        {
            playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-6.7f, 4.4f, 10.0f));
            stageManager.savedHalfPoint_ = {}; // 保存した中間地点リセット
        }
        playerManager.Initialize();
    }

    // enemy初期化  
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        SetEnemies(enemyManager);
        enemyManager.Initialize();
    }

    // 背景
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 5, 27));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 19, 15));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0, 0));

    // UI
    {
        UIManager::Instance().GetUI(UISPRITE::BaseMameHp)->SetPosition(playerUiPos);
        UIManager::Instance().GetUI(UISPRITE::BaseMameHp)->SetSize(playerUiSize);
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetPosition(playerUiPos);
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetSize(playerUiSize);
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetPosition(playerUiPos);
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetSize(playerUiSize);
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetPosition(playerUiPos);
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetSize(playerUiSize);

        UIManager::Instance().GetUI(UISPRITE::BaseMameHp)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetIsRender(true);

        UIManager::Instance().Initialize();
    }

    // BGM再生
    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::StageEx,      true);  // ExステージBGM再生
    audioManager.PlayBGM(BGM::StageEx_Back, true);  // Exステージの環境音BGM再生
}

void StageEx::Finalize()
{
    // エネミー終了化
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear();

    // player終了化
    PlayerManager::Instance().Finalize();

    // terrain終了化
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear();

    // uimanager
    UIManager::Instance().Finalize();
    UIManager::Instance().Clear();

    // 全音楽停止
    AudioManager::Instance().StopAllAudio();
}

void StageEx::Begin()
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        // player
        PlayerManager::Instance().Begin();

        // enemy
        EnemyManager::Instance().Begin();
    }
}

void StageEx::Update(const float& elapsedTime)
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        EnemyManager& enemyManager = EnemyManager::Instance();
        StageManager& stageManager = StageManager::Instance();


        //背景
        back->BackUpdate(elapsedTime);

        // camera更新
        Camera::Instance().UpdateEx(elapsedTime);

        // terrain更新
        TerrainManager::Instance().Update(elapsedTime);

        // player更新
        playerManager.Update(elapsedTime);

        // enemy更新
#if _DEBUG
        //static bool isEnemyUpdate = true;
        //ImGui::Begin("isEnemyUpdate0000000000000");
        //ImGui::Checkbox("isEnemyUpdate", & isEnemyUpdate);
        //ImGui::End();
        //if (isEnemyUpdate) enemyManager.Update(elapsedTime);
#endif
        enemyManager.Update(elapsedTime);


        Transform* playerTransform = playerManager.GetPlayer()->GetTransform();

        // 中間地点処理
        //const DirectX::XMFLOAT3 halfwayPointPos = halfwayPoint_->GetTransform()->GetPosition();
        //{
        //    // 中間地点を超えたらその中間地点情報を保存
        //    if (!isArrivedHalfwayPoint_)
        //    {
        //        if (playerTransform->GetPosition().x >= halfwayPointPos.x)
        //        {
        //            stageManager.savedHalfPoint_.savedHalfwayPointStage = StageManager::SavedHalfwayPointStage::StagePlains;
        //            stageManager.savedHalfPoint_.position = halfwayPointPos;
        //            isArrivedHalfwayPoint_ = true;
        //        }
        //    }
        //    // 旗を回転させる
        //    else
        //    {
        //        NO_CONST float halfwayPointRotationY = halfwayPoint_->GetTransform()->GetRotation().y;
        //        if (halfwayPointRotationY < 0.0f)
        //        {
        //            const float AddRotationY = ToRadian(540.) * elapsedTime;
        //            halfwayPointRotationY = (std::min)(0.0f, halfwayPointRotationY + AddRotationY);
        //        }
        //        halfwayPoint_->GetTransform()->SetRotationY(halfwayPointRotationY);
        //    }
        //}

        // プレイヤーがゴールを超えたらステージクリアにする
        //const float goalPositionX = goal_->GetTransform()->GetPosition().x;
        //{
        //    if (!isStageClear_)
        //    {
        //        if (playerTransform->GetPosition().x >= goalPositionX)
        //        {
        //            enemyManager.AllKill(); // 敵を全員倒す
        //            isStageClear_ = true;
        //        }
        //    }
        //    else
        //    {
        //        // プレイヤーのステートをクリアステートへ遷移
        //        const Player::State playerState = playerManager.GetPlayer()->GetState();
        //        if (playerState != Player::State::Clear) playerManager.GetPlayer()->TransitionClearState();

        //        // 位置を修正
        //        if (playerManager.GetPlayer()->GetClearState() != ClearState::MoveToRight)
        //        {
        //            NO_CONST float playerPositionX = playerTransform->GetPosition().x;

        //            const float targetPositionX = goalPositionX + 3.0f;
        //            const float addPositionX = 10.0f * elapsedTime;
        //            if (playerPositionX > targetPositionX)
        //            {
        //                playerPositionX = (std::max)(targetPositionX, (playerPositionX - addPositionX));
        //            }
        //            else if (playerPositionX < targetPositionX)
        //            {
        //                playerPositionX = (std::min)(targetPositionX, (playerPositionX + addPositionX));
        //            }

        //            playerTransform->SetPositionX(playerPositionX);
        //        }
        //        // 右に走り去っていったらステージセレクトに切り替える
        //        else
        //        {
        //            const float moveLimitX = goalPositionX + 10.0f;
        //            if (playerTransform->GetPosition().x > moveLimitX)
        //            {
        //                playerTransform->SetPositionX(moveLimitX);

        //                stageManager.savedHalfPoint_ = {}; // 保存した中間地点リセット

        //                Mame::Scene::SceneManager::Instance().selectState =
        //                    (std::max)(Mame::Scene::SceneManager::Instance().selectState, 2);
        //                StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
        //            }
        //        }
        //    }
        //}

        // 死亡処理
        if (playerTransform->GetPosition().z <= -50.0f ||
            playerTransform->GetPosition().y <= -50.0f)
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
        }

    }

    //Camera::Instance().GetTransform()->SetPosition(DirectX::XMFLOAT3(78.0f, 8.0f, -12.0f));
    //PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(78.0f, 8.0f, -12.0f));

    // UI
    UIManager::Instance().Update(elapsedTime);
    PlayerHpUiUpdate(elapsedTime);
}

void StageEx::End()
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        // player
        PlayerManager::Instance().End();

        // enemy
        EnemyManager::Instance().End();
    }
}

void StageEx::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // terrain描画
    TerrainManager::Instance().Render(elapsedTime);

    // 背景
    back->Render(elapsedTime);

    // player描画
    PlayerManager::Instance().Render(elapsedTime);

    // enemy描画
    EnemyManager::Instance().Render(elapsedTime);

    // UI
    UIManager::Instance().Render(elapsedTime);
}

void StageEx::DrawDebug()
{
#ifdef USE_IMGUI
    // terrain
    TerrainManager::Instance().DrawDebug();

    // player
    PlayerManager::Instance().DrawDebug();

    // enemy
    EnemyManager::Instance().DrawDebug();

    // ui
    UIManager::Instance().DrawDebug();

    // 背景
    back->DrawDebug();
#endif
}

void StageEx::RegisterTerrains(TerrainManager& terrainManager)
{
    terrainManager.Register(new TerrainNormal("./resources/stage/5.fbx"));  // 0
    terrainManager.Register(new TerrainNormal("./resources/stage/2.fbx"));  // 1

    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx"));    // 2
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx"));    // 3
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx"));    // 4
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx"));    // 5

    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 6
    terrainManager.Register(new TerrainNormal("./resources/stage/5.fbx"));  // 7
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 8
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 9
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 10
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 11

    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx"));    // 12
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx"));    // 13
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx"));    // 14
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx"));    // 15
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx"));    // 16
    
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 17
    terrainManager.Register(new TerrainNoClimb("./resources/stage/3.fbx"));    // 18
    terrainManager.Register(new TerrainNormal("./resources/stage/7.fbx"));   // 19
    terrainManager.Register(new TerrainNormal("./resources/stage/7.fbx"));    // 20
    terrainManager.Register(new TerrainDelayFall("./resources/stage/7.fbx")); // 21
    terrainManager.Register(new TerrainDelayFall("./resources/stage/7.fbx")); // 22
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 23
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 24
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 25

    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx")); // 26
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx")); // 27
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx")); // 28
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 29
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 30
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 31

    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx")); // 32
    terrainManager.Register(new TerrainNormal("./resources/stage/2.fbx"));    // 33
    terrainManager.Register(new TerrainNormal("./resources/stage/5.fbx"));    // 34
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx")); // 35
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx")); // 36
    terrainManager.Register(new TerrainDelayFall("./resources/stage/8.fbx")); // 37
    terrainManager.Register(new TerrainNoClimb("./resources/stage/4.fbx"));    // 38
    terrainManager.Register(new TerrainNormal("./resources/stage/2.fbx"));    // 39

    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx")); // 40
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx")); // 41
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx")); // 41
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx")); // 41
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx")); // 41
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx")); // 41
}

void StageEx::RegisterEnemies(EnemyManager& enemyManager)
{
    enemyManager.Register(new EnemyTofu());                 // 0
    enemyManager.Register(new EnemyTofu());                 // 1
    enemyManager.Register(new EnemyTofu());                 // 2
    enemyManager.Register(new EnemyTofu());                 // 3
    enemyManager.Register(new EnemyTofu());                 // 4
    enemyManager.Register(new EnemyTofu());                 // 5
    enemyManager.Register(new EnemyTofu());                 // 6

    enemyManager.Register(new EnemyTofu());                 // 7
    enemyManager.Register(new EnemyTofu());                 // 8
    enemyManager.Register(new EnemyTofu());                 // 9

    enemyManager.Register(new RED_TOFU::EnemyRedTofu());    // 10
    enemyManager.Register(new RED_TOFU::EnemyRedTofu());    // 11
    enemyManager.Register(new EnemyTofu());                 // 12
    enemyManager.Register(new EnemyTofu());                 // 13
    enemyManager.Register(new EnemyTofu());                 // 14
    enemyManager.Register(new EnemyTofu());                 // 15
    enemyManager.Register(new EnemyTofu());                 // 16
    
    enemyManager.Register(new EnemyTofu());                 // 17
    enemyManager.Register(new EnemyTofu());                 // 18
    enemyManager.Register(new EnemyTofu());                 // 19
    
    enemyManager.Register(new EnemyTofu());                 // 20
    
    enemyManager.Register(new EnemyTofu());                 // 21

    enemyManager.Register(new EnemyTofu());                 // 22
    enemyManager.Register(new EnemyTofu());                 // 23
    enemyManager.Register(new EnemyTofu());                 // 24
    
    enemyManager.Register(new EnemyTofu());                 // 25
    enemyManager.Register(new EnemyTofu());                 // 26
    
    enemyManager.Register(new EnemyTofu());                 // 27
    enemyManager.Register(new EnemyTofu());                 // 28


}

void StageEx::SetTerrains(TerrainManager& terrainManager)
{
    terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7.0f, 0.0f, 10.0f));
    terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(6.0f, -2.0f, 10.0f));

    terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f,  3.98f, 10.0f));
    terrainManager.GetTerrain(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(4.0f,  3.98f, 10.0f));
    terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(8.0f,  3.98f, 10.0f));
    terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(12.0f, 3.98f, 10.0f));

    terrainManager.GetTerrain(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(17.0f, -3.0f, 10.0f));
    terrainManager.GetTerrain(7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(23.5f, 0.0f, 10.0f));
    terrainManager.GetTerrain(8)->GetTransform()->SetPosition(DirectX::XMFLOAT3(30.3f, 1.0f, 10.0f));
    terrainManager.GetTerrain(9)->GetTransform()->SetPosition(DirectX::XMFLOAT3(35.0f, 3.0f, 10.0f));
    terrainManager.GetTerrain(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3(39.5f, -2.0f, 10.0f));
    terrainManager.GetTerrain(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(44.0f, 0.0f, 10.0f));
    
    terrainManager.GetTerrain(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(48.5f, 3.98f, 10.0f));
    terrainManager.GetTerrain(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(52.5f, 3.98f, 10.0f));
    terrainManager.GetTerrain(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(56.5f, 3.98f, 10.0f));
    terrainManager.GetTerrain(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(60.5f, 3.98f, 10.0f));
    terrainManager.GetTerrain(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(64.5f, 3.98f, 10.0f));

    terrainManager.GetTerrain(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(69.0f, 0.0f, 10.0f));
    terrainManager.GetTerrain(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(73.0f, 1.0f, 10.0f));
    terrainManager.GetTerrain(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(79.5f, 4.98f, 10.0f));
    terrainManager.GetTerrain(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(87.5f, 4.98f, 10.0f));
    terrainManager.GetTerrain(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(79.5f, 0.0f, 10.0f));
    terrainManager.GetTerrain(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(87.5f, 0.0f, 10.0f));
    terrainManager.GetTerrain(23)->GetTransform()->SetPosition(DirectX::XMFLOAT3(93.5f, -2.0f, 10.0f));
    terrainManager.GetTerrain(24)->GetTransform()->SetPosition(DirectX::XMFLOAT3(97.8f, 0.0f, 10.0f));
    terrainManager.GetTerrain(25)->GetTransform()->SetPosition(DirectX::XMFLOAT3(102.0f, 2.0f, 10.0f));

    terrainManager.GetTerrain(26)->GetTransform()->SetPosition(DirectX::XMFLOAT3(106.6f, 5.98f, 10.0f));
    terrainManager.GetTerrain(27)->GetTransform()->SetPosition(DirectX::XMFLOAT3(110.6f, 5.98f, 10.0f));
    terrainManager.GetTerrain(28)->GetTransform()->SetPosition(DirectX::XMFLOAT3(114.6f, 5.98f, 10.0f));
    
    terrainManager.GetTerrain(29)->GetTransform()->SetPosition(DirectX::XMFLOAT3(119.0f, 2.0f, 10.0f));
    terrainManager.GetTerrain(30)->GetTransform()->SetPosition(DirectX::XMFLOAT3(123.3f, -4.0f, 10.0f));
    terrainManager.GetTerrain(31)->GetTransform()->SetPosition(DirectX::XMFLOAT3(127.5f, -2.0f, 10.0f));
    terrainManager.GetTerrain(32)->GetTransform()->SetPosition(DirectX::XMFLOAT3(132.0f, 1.98f, 10.0f));
    terrainManager.GetTerrain(33)->GetTransform()->SetPosition(DirectX::XMFLOAT3(143.0f, 0.193f, 10.0f));
    
    terrainManager.GetTerrain(34)->GetTransform()->SetPosition(DirectX::XMFLOAT3(138.5f, 11.0f, 10.0f));
    terrainManager.GetTerrain(35)->GetTransform()->SetPosition(DirectX::XMFLOAT3(150.0f, 4.5f, 10.0f));
    terrainManager.GetTerrain(36)->GetTransform()->SetPosition(DirectX::XMFLOAT3(145.0f, 7.5f, 10.0f));
    terrainManager.GetTerrain(37)->GetTransform()->SetPosition(DirectX::XMFLOAT3(150.0f, 10.5f, 10.0f));
    terrainManager.GetTerrain(38)->GetTransform()->SetPosition(DirectX::XMFLOAT3(154.3f, 6.0f, 10.1f));
    
    terrainManager.GetTerrain(39)->GetTransform()->SetPosition(DirectX::XMFLOAT3(160.8f, 0.193f, 10.0f));
    terrainManager.GetTerrain(40)->GetTransform()->SetPosition(DirectX::XMFLOAT3(160.0f, 10.5f, 10.0f));
    terrainManager.GetTerrain(41)->GetTransform()->SetPosition(DirectX::XMFLOAT3(165.0f, 8.5f, 10.0f));

}

void StageEx::SetEnemies(EnemyManager& enemyManager)
{
    enemyManager.GetEnemy(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
    enemyManager.GetEnemy(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(2.0f, 0.0f, 10.0f));
    enemyManager.GetEnemy(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(4.0f, 0.0f, 10.0f));
    enemyManager.GetEnemy(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(6.0f, 0.0f, 10.0f));
    enemyManager.GetEnemy(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(8.0f, 0.0f, 10.0f));
    enemyManager.GetEnemy(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(10.0f, 0.0f, 10.0f));
    enemyManager.GetEnemy(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(12.0f, 0.0f, 10.0f));

    enemyManager.GetEnemy(7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(38.2f, 2.4f, 10.0f));
    enemyManager.GetEnemy(8)->GetTransform()->SetPosition(DirectX::XMFLOAT3(39.2f, 2.4f, 10.0f));
    enemyManager.GetEnemy(9)->GetTransform()->SetPosition(DirectX::XMFLOAT3(40.2f, 2.4f, 10.0f));

    enemyManager.GetEnemy(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3(76.7f, 0.5f, 10.0f));
    enemyManager.GetEnemy(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(79.7f, 0.5f, 10.0f));
    enemyManager.GetEnemy(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(80.0f, 0.5f, 10.0f));
    enemyManager.GetEnemy(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(82.0f, 0.5f, 10.0f));
    enemyManager.GetEnemy(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(84.0f, 0.5f, 10.0f));
    enemyManager.GetEnemy(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(86.0f, 0.5f, 10.0f));
    enemyManager.GetEnemy(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(88.0f, 0.5f, 10.0f));
    
    enemyManager.GetEnemy(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(78.0f, 5.4f, 10.0f));
    enemyManager.GetEnemy(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(83.0f, 5.4f, 10.0f));
    enemyManager.GetEnemy(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(88.0f, 5.4f, 10.0f));

    enemyManager.GetEnemy(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(97.0f, 4.4f, 10.0f));

    enemyManager.GetEnemy(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(110.0f, 6.4f, 10.0f));
    
    enemyManager.GetEnemy(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(122.0f, 0.4f, 10.0f));
    enemyManager.GetEnemy(23)->GetTransform()->SetPosition(DirectX::XMFLOAT3(123.0f, 0.4f, 10.0f));
    enemyManager.GetEnemy(24)->GetTransform()->SetPosition(DirectX::XMFLOAT3(124.0f, 0.4f, 10.0f));

    enemyManager.GetEnemy(25)->GetTransform()->SetPosition(DirectX::XMFLOAT3(140.0f, 1.0f, 10.0f));
    enemyManager.GetEnemy(26)->GetTransform()->SetPosition(DirectX::XMFLOAT3(143.0f, 1.0f, 10.0f));
    
    enemyManager.GetEnemy(27)->GetTransform()->SetPosition(DirectX::XMFLOAT3(160.0f, 11.0f, 10.0f));
    enemyManager.GetEnemy(28)->GetTransform()->SetPosition(DirectX::XMFLOAT3(165.0f, 9.0f, 10.0f));
}

void StageEx::PlayerHpUiUpdate(float elapsedTime)
{
    int health = PlayerManager::Instance().GetPlayer()->GetHealth();

    if (UIManager::Instance().GetUiCenter())
    {
        SetUiState();
        UIManager::Instance().SetUiCenter(false);
    }
    UpdateUi(3, 1.0f, uiState, elapsedTime);


    switch (health)
    {
    case 0:
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetIsRender(false);
        break;
    case 1:
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetIsRender(false);
        break;
    case 2:
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetIsRender(false);
        break;
    }
}

void StageEx::UpdateUi(int uiCount, float speed, int state, float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // ビューポート
    NO_CONST D3D11_VIEWPORT viewport = {};
    NO_CONST UINT			numViewports = 1;
    graphics.GetDeviceContext()->RSGetViewports(&numViewports, &viewport);

    // 変換行列
    const DirectX::XMMATRIX View = Camera::Instance().GetV();
    const DirectX::XMMATRIX Projection = Camera::Instance().GetP();
    const DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    switch (state)
    {
    case 0:
        SetUiTimer(0.6f);
        uiState = 1;
        break;
    case 1:
        for (int i = 0; i < uiCount; ++i)
        {
            DirectX::XMFLOAT3 pos = UIManager::Instance().GetUI(i)->GetPosition();
            DirectX::XMFLOAT2 size = UIManager::Instance().GetUI(i)->GetSize();



            // スクリーン座標変換
            NO_CONST DirectX::XMFLOAT3 screenPlPos = {};
            {
                // プレイヤーのワールド座標を取得
                NO_CONST DirectX::XMFLOAT3 worldPlPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
                worldPlPos.y += 3.0f; // プレイヤーの頭上を座標に設定

                // ワールド座標をスクリーン座標に変換
                const DirectX::XMVECTOR WorldPlPos = DirectX::XMLoadFloat3(&worldPlPos);
                const DirectX::XMVECTOR ScreenPlPos = {
                    DirectX::XMVector3Project(
                        WorldPlPos,
                        viewport.TopLeftX, viewport.TopLeftY,
                        viewport.Width,    viewport.Height,
                        viewport.MinDepth, viewport.MaxDepth,
                        Projection, View, World
                    )
                };

                // XMFLOAT型に変換
                DirectX::XMStoreFloat3(&screenPlPos, ScreenPlPos);
            }

            // スプライトの中心座標を左上から中央に調整
            const float sizeHalfX = size.x * 0.5f;
            const float sizeHalfY = size.y * 0.5f;
            displayUiPosition = { (screenPlPos.x - sizeHalfX), (screenPlPos.y - sizeHalfY),0 };


            //pos = { 450.0f, 260.0f };
            //UIManager::Instance().GetUI(i)->SetPosition(pos);

            UIManager::Instance().GetUI(i)->SetPosition(displayUiPosition);
        }
        subtractUiTimer(elapsedTime);
        if (GetUiTimer() <= 0.0f)uiState = 2;

        break;
    case 2:
        for (int i = 0; i < uiCount; ++i)
        {
            DirectX::XMFLOAT3 pos = UIManager::Instance().GetUI(i)->GetPosition();
            DirectX::XMFLOAT2 size = UIManager::Instance().GetUI(i)->GetSize();

            pos.x -= speed * 1.76f;
            pos.y -= speed;
            size.x -= speed * 2.15f;
            size.y -= speed;

            if (pos.x <= 10.0f)pos.x = 10.0f;
            if (pos.y <= 10.0f)pos.y = 10.0f;
            if (size.x <= 334.0f)size.x = 334.0f;
            if (size.y <= 160.0f)size.y = 160.0f;

            UIManager::Instance().GetUI(i)->SetPosition(pos);
            UIManager::Instance().GetUI(i)->SetSize(size);

            if ((pos.x <= 10.0f) && (pos.y <= 10.0f) && (size.x <= 334.0f) && (size.y <= 160.0f))
            {
                uiState = -1;
            }
        }
        break;
    }
}
