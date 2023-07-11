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
#include "StageEx.h"

// �R���X�g���N�^
StageSelection::StageSelection()
{
}

// ������
void StageSelection::Initialize()
{
    StageManager& stageManager = StageManager::Instance();

    // �X�e�[�W�̑�����ݒ�
    SetStageType(static_cast<int>(Mame::Stage::TYPE::SELECT));

    // ����
    {
        //stageManager.spriteDissolve = std::make_unique<SpriteDissolve>();

        // �w�i
        //back = std::make_unique<Box>("./resources/tutorialBack1.fbx");
        back = std::make_unique<Box>("./resources/tutorialBack.fbx");

        // ��
        castle = std::make_unique<Box>("./resources/castel.fbx");

        // boss
        boss = std::make_unique<Box>("./resources/bossall.fbx");

        Ex = std::make_unique<Box>("./resources/stage/8.fbx");
        signBoard = std::make_unique<Box>("./resources/stage/flag_yazi.fbx");

        // point
        point[POINT::Black] = std::make_unique<Box>("./resources/select/pointBlack.fbx");
        point[POINT::Red] = std::make_unique<Box>("./resources/select/pointRed.fbx");
        point[POINT::Blue] = std::make_unique<Box>("./resources/select/pointBlue.fbx");

        // terrain����
        TerrainManager& terrainManager = TerrainManager::Instance();
        {
            terrainManager.Register(new TerrainNormal("./resources/stage/1.fbx")); // 0
            terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx")); // 1
        }

        // player����
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();
    }


    //stageManager.spriteDissolve->Initialize();
    //stageManager.spriteDissolve->SetFadeInTexture({ 0,0,0 }, { 1280,720 }, 0.4f, 2);

    // camera������
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 16, -20));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(30), 0, 0, 0));
    camera.Initialize();

    // �w�i
    {
        back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, -1, 20));
        back->GetTransform()->SetScale(DirectX::XMFLOAT3(50, 30, 8));
        //back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0, 0));
    }

    // ��
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

    // �_
    {
        Ex->GetTransform()->SetPosition(DirectX::XMFLOAT3(14, 5, 2));
    }

    // �Ŕ�
    {
        signBoard->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 3, 1));
        signBoard->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(180), 0, 0));
    }

    // point
    {
        point[POINT::Black]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 2.2f, 0.0f));
        point[POINT::Red]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 2.2f, 0.0f));
        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 2.2f, 0.0f));
    }

    // terrain������
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        // ���ׂ�
        {
            terrainManager.GetTerrain(0)->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 1, 2));

            terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 6, 2));
            terrainManager.GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
            terrainManager.GetTerrain(1)->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(-15), 0, 0, 0));
        }
        terrainManager.Initialize();
    }

    // player������
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7.0, 2.4f, 0.0f));
        //playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 1.5f, 0.0f));
        playerManager.Initialize();
    }

    // BGM�Đ�
    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::StageSelection, true);  // �X�e�[�W�Z���N�gBGM�Đ�
}

// �I����
void StageSelection::Finalize()
{
    // terrain�I����
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear();

    // player�I����
    PlayerManager::Instance().Finalize();

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopAllAudio(); // �S���y��~
}

// Update�̑O�ɌĂ΂�鏈��
void StageSelection::Begin()
{
}

// �X�V����
void StageSelection::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    AudioManager& audioManager = AudioManager::Instance();

    StageManager& stageManager = StageManager::Instance();

    //stageManager.spriteDissolve->Update();

    // boss
    boss->SelectBossUpdate(elapsedTime);

    // terrain�X�V
    TerrainManager::Instance().Update(elapsedTime);

    // player�X�V
    PlayerManager::Instance().UpdateSelectStage(elapsedTime, &selectState);

    DirectX::XMFLOAT3 cameraPos = Camera::Instance().GetTransform()->GetPosition();
    if (selectState <= SELECT::PlainsStage)
    {
        cameraPos.x -= elapsedTime * 10;
        if (cameraPos.x <= 0.0f)cameraPos.x = 0.0f;
    }
    if (selectState >= SELECT::BossStage)
    {
        cameraPos.x += elapsedTime * 10;
        if (cameraPos.x >= 5.0f)cameraPos.x = 5.0f;
    }
    Camera::Instance().GetTransform()->SetPosition(cameraPos);

    // �X�e�[�W�I��
    switch (selectState)
    {
    case SELECT::TutorialStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
        Ex->GetTransform()->SetScale(DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f));
        
        // tutorialStage��
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StageTutorial));

            audioManager.StopSE(SE::Select);
            audioManager.PlaySE(SE::Select, false, true); // �I��SE�Đ�
            break;
        }

        break;
    case SELECT::PlainsStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.4f, 0.4f, 0.4f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
        Ex->GetTransform()->SetScale(DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f));

        // plainsStage��
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StagePlains));

            audioManager.StopSE(SE::Select);
            audioManager.PlaySE(SE::Select, false, true); // �I��SE�Đ�

            break;
        }

        break;
    case SELECT::BossStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.3f, 1.3f, 1.3f));
        Ex->GetTransform()->SetScale(DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f));

        // bossStage��
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StageBoss));

            audioManager.StopSE(SE::Select);
            audioManager.PlaySE(SE::Select, false, true); // �I��SE�Đ�

            break;
        }

        break;
    case SELECT::ExStage:
        castle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
        boss->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
        Ex->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

        // exStage��
        if (gamePad.GetButtonDown() & (GamePad::BTN_A))
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StageEx));

            audioManager.StopSE(SE::Select);
            audioManager.PlaySE(SE::Select, false, true); // �I��SE�Đ�

            break;
        }

        break;
    }

    // ��]
    {
        float rotSpeed = 30 * elapsedTime;

        DirectX::XMFLOAT4 bossRot = boss->GetTransform()->GetRotation();
        DirectX::XMFLOAT4 castleRot = castle->GetTransform()->GetRotation();
        DirectX::XMFLOAT4 terrainRot = TerrainManager::Instance().GetTerrain(1)->GetTransform()->GetRotation();
        DirectX::XMFLOAT4 exRot = Ex->GetTransform()->GetRotation();

        bossRot.y += DirectX::XMConvertToRadians(rotSpeed);
        castleRot.y += DirectX::XMConvertToRadians(rotSpeed);
        terrainRot.y += DirectX::XMConvertToRadians(rotSpeed);
        exRot.y += DirectX::XMConvertToRadians(rotSpeed);

        if (bossRot.y >= DirectX::XMConvertToRadians(360))bossRot.y -= DirectX::XMConvertToRadians(360);
        if (castleRot.y >= DirectX::XMConvertToRadians(360))castleRot.y -= DirectX::XMConvertToRadians(360);
        if (terrainRot.y >= DirectX::XMConvertToRadians(360))terrainRot.y -= DirectX::XMConvertToRadians(360);
        if (exRot.y >= DirectX::XMConvertToRadians(360))exRot.y -= DirectX::XMConvertToRadians(360);

        boss->GetTransform()->SetRotation(bossRot);
        castle->GetTransform()->SetRotation(castleRot);
        TerrainManager::Instance().GetTerrain(1)->GetTransform()->SetRotation(terrainRot);
        Ex->GetTransform()->SetRotation(exRot);
    }
}

// Update�̌�ɌĂ΂�鏈��
void StageSelection::End()
{
}

// �`�揈��
void StageSelection::Render(const float& elapsedTime)
{
    StageManager& stageManager = StageManager::Instance();

    // �w�i
    back->Render(elapsedTime, true, 1.0f);

    // ��
    castle->Render(elapsedTime);

    // boss
    boss->Render(elapsedTime);
    Ex->Render(elapsedTime);
    signBoard->Render(elapsedTime);

    // point
    PointRender(elapsedTime);

    // terrain
    TerrainManager::Instance().Render(elapsedTime);

    // player�`��
    PlayerManager::Instance().Render(elapsedTime);
}

// debug�p
void StageSelection::DrawDebug()
{
#ifdef USE_IMGUI
    // �w�i
    back->DrawDebug();

    // ��
    castle->DrawDebug();

    // boss
    boss->DrawDebug();

    Ex->DrawDebug();
    signBoard->DrawDebug();

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

        point[POINT::Black]->GetTransform()->SetPosition(DirectX::XMFLOAT3(14, 2.2f, 0));
        point[POINT::Black]->Render(elapsedTime, 1.0f);

        break;
    case POINTSTATE::Plains:
        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Red]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2.2f, 0));
        point[POINT::Red]->Render(elapsedTime, 1.0f);

        point[POINT::Black]->GetTransform()->SetPosition(DirectX::XMFLOAT3(7, 2.2f, 0));
        point[POINT::Black]->Render(elapsedTime, 1.0f);

        point[POINT::Black]->GetTransform()->SetPosition(DirectX::XMFLOAT3(14, 2.2f, 0));
        point[POINT::Black]->Render(elapsedTime, 1.0f);

        break;
    case POINTSTATE::Boss:
        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Red]->GetTransform()->SetPosition(DirectX::XMFLOAT3(7, 2.2f, 0));
        point[POINT::Red]->Render(elapsedTime, 1.0f);

        point[POINT::Black]->GetTransform()->SetPosition(DirectX::XMFLOAT3(14, 2.2f, 0));
        point[POINT::Black]->Render(elapsedTime, 1.0f);

        break;
    case POINTSTATE::EX:
        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(7, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Red]->GetTransform()->SetPosition(DirectX::XMFLOAT3(14, 2.2f, 0));
        point[POINT::Red]->Render(elapsedTime, 1.0f);
    case POINTSTATE::Clear:
        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(7, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);

        point[POINT::Blue]->GetTransform()->SetPosition(DirectX::XMFLOAT3(14, 2.2f, 0));
        point[POINT::Blue]->Render(elapsedTime, 1.0f);
        break;
    }
}
