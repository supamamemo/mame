#include "StageSelection.h"

#include "../../Mame/Graphics/Graphics.h"
#include "../../Mame/Graphics/Camera.h"
#include "../../Mame/Scene/SceneManager.h"
#include "../../Mame/Input/Input.h"
#include "../../Mame/AudioManager.h"

#include "../EnemyManager.h"
#include "../Terrain/TerrainManager.h"
#include "../Terrain/TerrainNormal.h"
#include "../PlayerManager.h"

#include "StageManager.h"
#include "StageLoading.h"
#include "StageTutorial.h"
#include "StagePlains.h"
#include "StageBoss.h"

// コンストラクタ
StageSelection::StageSelection()
{
}

// 初期化
void StageSelection::Initialize()
{
    // 生成
    {
        // 背景
        //back = std::make_unique<Box>("./resources/tutorialBack1.fbx");
        back = std::make_unique<Box>("./resources/tutorialBack.fbx");

        // 城
        castle = std::make_unique<Box>("./resources/castel.fbx");

        // boss
        boss = std::make_unique<Box>("./resources/bossall.fbx");

        // point
        point[POINT::Black] = std::make_unique<Box>("./resources/select/pointBlack.fbx");
        point[POINT::Red] = std::make_unique<Box>("./resources/select/pointRed.fbx");
        point[POINT::Blue] = std::make_unique<Box>("./resources/select/pointBlue.fbx");

        // terrain生成
        TerrainManager& terrainManager = TerrainManager::Instance();
        {
            terrainManager.Register(new TerrainNormal("./resources/stage/1.fbx")); // 0
            terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx")); // 1
        }

        // player生成
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();
    }

    // camera初期化
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 16, -20));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(30), 0, 0, 0));

    // 背景
    {
        back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, -1, 20));
        back->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 22, 8));
        back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0, 0));
    }

    // 城
    {
        castle->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 6, 2));
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
        castle->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(-10), 0.0f, 0.0f, 0.0f));
    }

    // boss
    {
        boss->GetTransform()->SetPosition(DirectX::XMFLOAT3(7.0f, 4.5f, 2.0f));
        boss->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(-10), 0.0f, 0.0f, 0.0f));
        boss->SelectBossInitialize();
    }

    // point
    {
        point[POINT::Black]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 2.2f, 0.0f));
        point[POINT::Red]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 2.2f, 0.0f));
        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 2.2f, 0.0f));
    }

    // terrain初期化
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        // 並べる
        {
            terrainManager.GetTerrain(0)->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 1, 2));

            terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 6, 2));
            terrainManager.GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
            terrainManager.GetTerrain(1)->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(-15), 0, 0, 0));
        }
        terrainManager.Initialize();
    }

    // player初期化
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7.0, 2.4f, 0.0f));
        //playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 1.5f, 0.0f));
        playerManager.Initialize();
    }

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::Tutorial, true); // チュートリアルBGM再生
}

// 終了化
void StageSelection::Finalize()
{
    // terrain終了化
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear();

    // player終了化
    PlayerManager::Instance().Finalize();

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopAllAudio(); // 全音楽停止
}

// Updateの前に呼ばれる処理
void StageSelection::Begin()
{
}

// 更新処理
void StageSelection::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // boss
    boss->SelectBossUpdate(elapsedTime);

    // terrain更新
    TerrainManager::Instance().Update(elapsedTime);

    // player更新
    PlayerManager::Instance().UpdateSelectStage(elapsedTime, &selectState);

    // ステージ選択
    switch (selectState)
    {
    case SELECT::TutorialStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

        // tutorialStageへ
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StageTutorial));
        }

        break;
    case SELECT::PlainsStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.4f, 0.4f, 0.4f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

        // plainsStageへ
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StagePlains));
        }

        break;
    case SELECT::BossStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.3f, 1.3f, 1.3f));

        // bossStageへ
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StageBoss));
        }

        break;
    }

    // 回転
    {
        float rotSpeed = 30 * elapsedTime;

        DirectX::XMFLOAT4 bossRot = boss->GetTransform()->GetRotation();
        DirectX::XMFLOAT4 castleRot = castle->GetTransform()->GetRotation();
        DirectX::XMFLOAT4 terrainRot = TerrainManager::Instance().GetTerrain(1)->GetTransform()->GetRotation();

        bossRot.y += DirectX::XMConvertToRadians(rotSpeed);
        castleRot.y += DirectX::XMConvertToRadians(rotSpeed);
        terrainRot.y += DirectX::XMConvertToRadians(rotSpeed);

        if (bossRot.y >= DirectX::XMConvertToRadians(360))bossRot.y -= DirectX::XMConvertToRadians(360);
        if (castleRot.y >= DirectX::XMConvertToRadians(360))castleRot.y -= DirectX::XMConvertToRadians(360);
        if (terrainRot.y >= DirectX::XMConvertToRadians(360))terrainRot.y -= DirectX::XMConvertToRadians(360);

        boss->GetTransform()->SetRotation(bossRot);
        castle->GetTransform()->SetRotation(castleRot);
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetRotation(terrainRot);
    }
}

// Updateの後に呼ばれる処理
void StageSelection::End()
{
}

// 描画処理
void StageSelection::Render(const float& elapsedTime)
{
    // 背景
    back->Render(elapsedTime, true);

    // 城
    castle->Render(elapsedTime);

    // boss
    boss->Render(elapsedTime);

    // point
    PointRender(elapsedTime);

    // terrain
    TerrainManager::Instance().Render(elapsedTime);

    // player描画
    PlayerManager::Instance().Render(elapsedTime);
}

// debug用
void StageSelection::DrawDebug()
{
#ifdef USE_IMGUI
    // 背景
    back->DrawDebug();

    // 城
    castle->DrawDebug();

    // boss
    boss->DrawDebug();

    ImGui::SliderInt("state", &Mame::Scene::SceneManager::Instance().selectState, 0, 2);

    // terrain
    TerrainManager::Instance().DrawDebug();

    // player
    PlayerManager::Instance().DrawDebug();

#endif
}

void StageSelection::PointUpdate()
{
    switch (Mame::Scene::SceneManager::Instance().selectState)
    {
    case POINTSTATE::Tutorial:
        break;
    }
}

void StageSelection::PointRender(const float& elapsedTime)
{
    switch (Mame::Scene::SceneManager::Instance().selectState)
    {
    case POINTSTATE::Tutorial:
        point[POINT::Red]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 2.2f, 0));
        point[POINT::Red]->Render(elapsedTime, 1.0f);

        point[POINT::Black]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2.2f, 0));
        point[POINT::Black]->Render(elapsedTime, 1.0f);

        point[POINT::Black]->GetTransform()->SetPosition(DirectX::XMFLOAT3(7, 2.2f, 0));
        point[POINT::Black]->Render(elapsedTime, 1.0f);

        break;
    case POINTSTATE::Plains:
        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Red]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2.2f, 0));
        point[POINT::Red]->Render(elapsedTime, 1.0f);

        point[POINT::Black]->GetTransform()->SetPosition(DirectX::XMFLOAT3(7, 2.2f, 0));
        point[POINT::Black]->Render(elapsedTime, 1.0f);

        break;
    case POINTSTATE::Boss:
        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Red]->GetTransform()->SetPosition(DirectX::XMFLOAT3(7, 2.2f, 0));
        point[POINT::Red]->Render(elapsedTime, 1.0f);

        break;
    }
}
