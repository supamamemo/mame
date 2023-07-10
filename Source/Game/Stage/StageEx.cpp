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

    // �X�e�[�W�̑�����ݒ�
    SetStageType(static_cast<int>(Mame::Stage::TYPE::Ex));

    // ����
    {
        // terrain����
        Terrain::nameNum = 0;
        TerrainManager& terrainManager = TerrainManager::Instance();
        RegisterTerrains(terrainManager);

        // player����
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

        // enemy����
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

    // camera������
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 8.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0, 0, 0));

    // terrain������
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        SetTerrains(terrainManager);
        terrainManager.Initialize();
    }

    // player������
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        // �ۑ����Ă钆�Ԓn�_�������̃X�e�[�W�̒��Ԓn�_�������璆�Ԓn�_�̈ʒu���v���C���[�ɑ��
        if (stageManager.savedHalfPoint_.savedHalfwayPointStage == StageManager::SavedHalfwayPointStage::StagePlains)
        {
            playerManager.GetPlayer()->GetTransform()->SetPosition(stageManager.savedHalfPoint_.position);
            playerManager.GetPlayer()->GetTransform()->SetPositionZ(10.0f);
        }
        // �ۑ����Ă钆�Ԓn�_�������̃X�e�[�W�̒��Ԓn�_�łȂ���΃v���C���[�ɏ����ʒu�������A�ۑ��������Ԓn�_�������Z�b�g
        else
        {
            playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-6.7f, 4.4f, 10.0f));
            stageManager.savedHalfPoint_ = {}; // �ۑ��������Ԓn�_���Z�b�g
        }
        playerManager.Initialize();
    }

    // enemy������  
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        SetEnemies(enemyManager);
        enemyManager.Initialize();
    }

    // �w�i
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

    // BGM�Đ�
    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::StagePlains, true);  // �����X�e�[�WBGM�Đ�
    audioManager.PlayBGM(BGM::StagePlains_Back, true);  // �����X�e�[�W�̊���BGM�Đ�
}

void StageEx::Finalize()
{
    // �G�l�~�[�I����
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear();

    // player�I����
    PlayerManager::Instance().Finalize();

    // terrain�I����
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear();

    // uimanager
    UIManager::Instance().Finalize();
    UIManager::Instance().Clear();

    // �S���y��~
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


        //�w�i
        back->BackUpdate(elapsedTime);

        // camera�X�V
        Camera::Instance().Update(elapsedTime);

        // terrain�X�V
        TerrainManager::Instance().Update(elapsedTime);

        // player�X�V
        playerManager.Update(elapsedTime);

        // enemy�X�V
        enemyManager.Update(elapsedTime);


        Transform* playerTransform = playerManager.GetPlayer()->GetTransform();

        // ���Ԓn�_����
        //const DirectX::XMFLOAT3 halfwayPointPos = halfwayPoint_->GetTransform()->GetPosition();
        //{
        //    // ���Ԓn�_�𒴂����炻�̒��Ԓn�_����ۑ�
        //    if (!isArrivedHalfwayPoint_)
        //    {
        //        if (playerTransform->GetPosition().x >= halfwayPointPos.x)
        //        {
        //            stageManager.savedHalfPoint_.savedHalfwayPointStage = StageManager::SavedHalfwayPointStage::StagePlains;
        //            stageManager.savedHalfPoint_.position = halfwayPointPos;
        //            isArrivedHalfwayPoint_ = true;
        //        }
        //    }
        //    // ������]������
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

        // �v���C���[���S�[���𒴂�����X�e�[�W�N���A�ɂ���
        //const float goalPositionX = goal_->GetTransform()->GetPosition().x;
        //{
        //    if (!isStageClear_)
        //    {
        //        if (playerTransform->GetPosition().x >= goalPositionX)
        //        {
        //            enemyManager.AllKill(); // �G��S���|��
        //            isStageClear_ = true;
        //        }
        //    }
        //    else
        //    {
        //        // �v���C���[�̃X�e�[�g���N���A�X�e�[�g�֑J��
        //        const Player::State playerState = playerManager.GetPlayer()->GetState();
        //        if (playerState != Player::State::Clear) playerManager.GetPlayer()->TransitionClearState();

        //        // �ʒu���C��
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
        //        // �E�ɑ��苎���Ă�������X�e�[�W�Z���N�g�ɐ؂�ւ���
        //        else
        //        {
        //            const float moveLimitX = goalPositionX + 10.0f;
        //            if (playerTransform->GetPosition().x > moveLimitX)
        //            {
        //                playerTransform->SetPositionX(moveLimitX);

        //                stageManager.savedHalfPoint_ = {}; // �ۑ��������Ԓn�_���Z�b�g

        //                Mame::Scene::SceneManager::Instance().selectState =
        //                    (std::max)(Mame::Scene::SceneManager::Instance().selectState, 2);
        //                StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
        //            }
        //        }
        //    }
        //}

        // ���S����
        if (playerTransform->GetPosition().z <= -50.0f ||
            playerTransform->GetPosition().y <= -50.0f)
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
        }

    }


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

    // terrain�`��
    TerrainManager::Instance().Render(elapsedTime);

    // �w�i
    back->Render(elapsedTime);

    // player�`��
    PlayerManager::Instance().Render(elapsedTime);

    // enemy�`��
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

    // �w�i
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
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 18
    terrainManager.Register(new TerrainNoClimb("./resources/stage/7.fbx"));   // 19
    terrainManager.Register(new TerrainNormal("./resources/stage/7.fbx"));    // 20
    terrainManager.Register(new TerrainDelayFall("./resources/stage/7.fbx")); // 21
    terrainManager.Register(new TerrainDelayFall("./resources/stage/7.fbx")); // 22
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 23
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 24
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));    // 25

}

void StageEx::RegisterEnemies(EnemyManager& enemyManager)
{
}

void StageEx::SetTerrains(TerrainManager& terrainManager)
{
    terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7.0f, 0.0f, 10.0f));
    terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(6.0f, -2.0f, 10.0f));

    terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 3.9f, 10.0f));
    terrainManager.GetTerrain(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(4.0f, 3.9f, 10.0f));
    terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(8.0f, 3.9f, 10.0f));
    terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(12.0f, 3.9f, 10.0f));

    terrainManager.GetTerrain(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(17.0f, -3.0f, 10.0f));
    terrainManager.GetTerrain(7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(23.5f, 0.0f, 10.0f));
    terrainManager.GetTerrain(8)->GetTransform()->SetPosition(DirectX::XMFLOAT3(30.3f, 1.0f, 10.0f));
    terrainManager.GetTerrain(9)->GetTransform()->SetPosition(DirectX::XMFLOAT3(35.0f, 3.0f, 10.0f));
    terrainManager.GetTerrain(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3(39.5f, -2.0f, 10.0f));
    terrainManager.GetTerrain(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(44.0f, 0.0f, 10.0f));
    
    terrainManager.GetTerrain(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(48.5f, 3.9f, 10.0f));
    terrainManager.GetTerrain(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(52.5f, 3.9f, 10.0f));
    terrainManager.GetTerrain(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(56.5f, 3.9f, 10.0f));
    terrainManager.GetTerrain(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(60.5f, 3.9f, 10.0f));
    terrainManager.GetTerrain(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(64.5f, 3.9f, 10.0f));

    terrainManager.GetTerrain(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(69.0f, 0.0f, 10.0f));
    terrainManager.GetTerrain(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(73.0f, 1.0f, 10.0f));
    terrainManager.GetTerrain(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(79.5f, 4.98f, 10.0f));
    terrainManager.GetTerrain(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(87.5f, 4.98f, 10.0f));
    terrainManager.GetTerrain(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(79.5f, 0.0f, 10.0f));
    terrainManager.GetTerrain(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(87.5f, 0.0f, 10.0f));
    terrainManager.GetTerrain(23)->GetTransform()->SetPosition(DirectX::XMFLOAT3(93.5f, -2.0f, 10.0f));
    terrainManager.GetTerrain(24)->GetTransform()->SetPosition(DirectX::XMFLOAT3(97.8f, 0.0f, 10.0f));
    terrainManager.GetTerrain(25)->GetTransform()->SetPosition(DirectX::XMFLOAT3(102.0f, 2.0f, 10.0f));


}

void StageEx::SetEnemies(EnemyManager& enemyManager)
{
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

    // �r���[�|�[�g
    NO_CONST D3D11_VIEWPORT viewport = {};
    NO_CONST UINT			numViewports = 1;
    graphics.GetDeviceContext()->RSGetViewports(&numViewports, &viewport);

    // �ϊ��s��
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



            // �X�N���[�����W�ϊ�
            NO_CONST DirectX::XMFLOAT3 screenPlPos = {};
            {
                // �v���C���[�̃��[���h���W���擾
                NO_CONST DirectX::XMFLOAT3 worldPlPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
                worldPlPos.y += 3.0f; // �v���C���[�̓�������W�ɐݒ�

                // ���[���h���W���X�N���[�����W�ɕϊ�
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

                // XMFLOAT�^�ɕϊ�
                DirectX::XMStoreFloat3(&screenPlPos, ScreenPlPos);
            }

            // �X�v���C�g�̒��S���W�����ォ�璆���ɒ���
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