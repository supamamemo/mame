#include "StageSelection.h"

#include "../../Mame/Graphics/Graphics.h"
#include "../../Mame/Graphics/Camera.h"
#include "../../Mame/Scene/SceneManager.h"
#include "../../Mame/Input/Input.h"

#include "../EnemyManager.h"
#include "../Terrain/TerrainManager.h"
#include "../Terrain/TerrainNormal.h"
#include "../PlayerManager.h"

// ÉRÉìÉXÉgÉâÉNÉ^
StageSelection::StageSelection()
{
    // îwåi
    back = std::make_unique<Box>("./resources/back.fbx");

    // èÈ
    castle = std::make_unique<Box>("./resources/castel.fbx");

    // boss
    boss = std::make_unique<Box>("./resources/bossall.fbx");

    // point
    point[POINT::Black] = std::make_unique<Box>("./resources/select/pointBlack.fbx");
    point[POINT::Red] = std::make_unique<Box>("./resources/select/pointRed.fbx");
    point[POINT::Blue] = std::make_unique<Box>("./resources/select/pointBlue.fbx");

    // terrainê∂ê¨
    TerrainManager& terrainManager = TerrainManager::Instance();
    {
        terrainManager.Register(new TerrainNormal("./resources/stage/1.fbx")); // 0
        terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx")); // 1
    }

    // playerê∂ê¨
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();
}

// èâä˙âª
void StageSelection::Initialize()
{
    // cameraèâä˙âª
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 16, -20));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(30), 0, 0, 0));

    // îwåi
    {
        back->GetTransform()->SetPosition(DirectX::XMFLOAT3(1, 5, 12));
        back->GetTransform()->SetScale(DirectX::XMFLOAT3(200, 800, 6));
        back->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 5.0f, 1.0f, 1.0f));
        //back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0, 0));
    }

    // èÈ
    {
        castle->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 6, 2));
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
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

    // terrainèâä˙âª
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        // ï¿Ç◊ÇÈ
        {
            terrainManager.GetTerrain(0)->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 1, 2));

            terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 6, 2));
            terrainManager.GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
            terrainManager.GetTerrain(1)->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(-15), 0, 0, 0));
        }
        terrainManager.Initialize();
    }

    // playerèâä˙âª
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7.0, 2.4f, 0.0f));
        //playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 1.5f, 0.0f));
        playerManager.Initialize();
    }
}

// èIóπâª
void StageSelection::Finalize()
{
    // terrainèIóπâª
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear();

    // playerèIóπâª
    PlayerManager::Instance().Finalize();
}

// UpdateÇÃëOÇ…åƒÇŒÇÍÇÈèàóù
void StageSelection::Begin()
{
}

// çXêVèàóù
void StageSelection::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // boss
    boss->SelectBossUpdate(elapsedTime);

    // terrainçXêV
    TerrainManager::Instance().Update(elapsedTime);

    // playerçXêV
    PlayerManager::Instance().UpdateSelectStage(elapsedTime, &selectState);

    // ÉXÉeÅ[ÉWëIë
    switch (selectState)
    {
    case SELECT::TutorialStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

        // tutorialStageÇ÷
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            Mame::Scene::SceneManager::Instance().GetCurrentScene()->ChangeStage(static_cast<int>(Mame::Scene::STAGE::Tutorial));
        }

        break;
    case SELECT::PlainsStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.4f, 0.4f, 0.4f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

        // plainsStageÇ÷
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            Mame::Scene::SceneManager::Instance().GetCurrentScene()->ChangeStage(static_cast<int>(Mame::Scene::STAGE::Plains));
        }

        break;
    case SELECT::BossStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.3f, 1.3f, 1.3f));

        // bossStageÇ÷
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            Mame::Scene::SceneManager::Instance().GetCurrentScene()->ChangeStage(static_cast<int>(Mame::Scene::STAGE::Boss));
        }

        break;
    }

    // âÒì]
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

// UpdateÇÃå„Ç…åƒÇŒÇÍÇÈèàóù
void StageSelection::End()
{
}

// ï`âÊèàóù
void StageSelection::Render(const float& elapsedTime)
{
    // îwåi
    back->Render(elapsedTime, true);

    // èÈ
    castle->Render(elapsedTime);

    // boss
    boss->Render(elapsedTime);

    // point
    PointRender(elapsedTime);

    // terrain
    TerrainManager::Instance().Render(elapsedTime);

    // playerï`âÊ
    PlayerManager::Instance().Render(elapsedTime);
}

// debugóp
void StageSelection::DrawDebug()
{
#ifdef USE_IMGUI
    // îwåi
    back->DrawDebug();

    // èÈ
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
