#include "StageTutorial.h"

#include "../../Mame/Graphics/Graphics.h"
#include "../../Mame/Input/Input.h"

#include "../PlayerManager.h"

#include "../EnemyTofu.h"
#include "../EnemyManager.h"

#include "../Terrain/TerrainBoss.h"
#include "../Terrain/TerrainPlains.h"
#include "../Terrain/TerrainManager.h"

#include "../UIManager.h"

#include "../Stage/StageManager.h"
#include "../Stage/StagePlains.h"

enum UISPRITE
{
    Bubble,         // �����o��
    BubbleStick,    // �����o��PC
    StickBase,      // base
    StickCenter,    // center
    StickRight,     // right
    StickLeft,      // left
    GamePadA,
    GamePadB,
    GamePadX,
    GamePadY,
    KeyBoardA,
    KeyBoardD,
    KeyBoardS,
    KeyBoardSHIFT,
    KeyBoardSPACE,
};

// �R���X�g���N�^
StageTutorial::StageTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // �X�e�[�W����&�o�^
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        
        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx"));  // 0
        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx"));  // 1
    }

    // player����
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // enemy
    EnemyTofu* tofu = new EnemyTofu();
    EnemyManager::Instance().Register(tofu);

    // �w�i��
    back = std::make_unique<Box>("./resources/back.fbx");

    // �Ŕ�
    signboard = std::make_unique<Box>("./resources/frag.fbx");

    // UI
    {
        UIManager& uiManager = UIManager::Instance();

        uiManager.Register(new UI(L"./resources/tutorial/hukidasi.png"));           // hukidasi
        uiManager.Register(new UI(L"./resources/tutorial/hukidasi_stick.png"));     // hukidasiPC
        uiManager.Register(new UI(L"./resources/tutorial/stickBase.png"));          // stickbase
        uiManager.Register(new UI(L"./resources/tutorial/stick_center.png"));       // center
        uiManager.Register(new UI(L"./resources/tutorial/stick_right.png"));        // right
        uiManager.Register(new UI(L"./resources/tutorial/stick_left.png"));         // left
        uiManager.Register(new UI(L"./resources/tutorial/GamePadA.png"));       // A
        uiManager.Register(new UI(L"./resources/tutorial/GamePadB.png"));       // B
        uiManager.Register(new UI(L"./resources/tutorial/GamePadX.png"));       // X
        uiManager.Register(new UI(L"./resources/tutorial/GamePadY.png"));       // Y
        uiManager.Register(new UI(L"./resources/tutorial/PC_A.png"));           // A_PC
        uiManager.Register(new UI(L"./resources/tutorial/PC_D.png"));           // D_PC
        uiManager.Register(new UI(L"./resources/tutorial/PC_S.png"));           // S_PC
        uiManager.Register(new UI(L"./resources/tutorial/PC_shift.png"));       // SHIFT
        uiManager.Register(new UI(L"./resources/tutorial/PC_space.png"));       // SPACE
    }
}

StageTutorial::~StageTutorial()
{

}

// ������
void StageTutorial::Initialize()
{
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0.0f, 0.0f, 0.0f));

    // �X�e�[�W�����ݒ�
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(32.0f, 0.0f, 10.0f));
        terrainManager.Initialize();
    }

    // player������
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-8.0f, 2.0f, 10.0f));
        playerManager.Initialize();
    }

    // enemy������
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        enemyManager.GetEnemy(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(7.5f, 1.5f, 10.0f));
        enemyManager.Initialize();
    }

    // �w�i��
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 4.0f, 32.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 22.0f, 10.0f));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0.0f, 0.0f));

    // �Ŕ�
    signboard->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 3.0f, 10.0f));

    // �U���p
    tutorialState = STATE::MoveReception;

    // UI
    {
        // stick
        {
            UIManager::Instance().GetUI(UISPRITE::StickBase)->SetPosition(DirectX::XMFLOAT2(160, 300));
            UIManager::Instance().GetUI(UISPRITE::StickBase)->SetSize(DirectX::XMFLOAT2(100, 100));

            UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetPosition(DirectX::XMFLOAT2(160, 300));
            UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetSize(DirectX::XMFLOAT2(100, 100));

            UIManager::Instance().GetUI(UISPRITE::StickRight)->SetPosition(DirectX::XMFLOAT2(160, 300));
            UIManager::Instance().GetUI(UISPRITE::StickRight)->SetSize(DirectX::XMFLOAT2(100, 100));

            UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetPosition(DirectX::XMFLOAT2(160, 300));
            UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetSize(DirectX::XMFLOAT2(100, 100));
        }

        UIManager::Instance().GetUI(UISPRITE::Bubble)->SetPosition(DirectX::XMFLOAT2(60, 280));
        UIManager::Instance().GetUI(UISPRITE::Bubble)->SetSize(DirectX::XMFLOAT2(300, 150));
        
        UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetPosition(DirectX::XMFLOAT2(100, 300));
        UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetSize(DirectX::XMFLOAT2(100, 100));
        UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetPosition(DirectX::XMFLOAT2(210, 300));
        UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetSize(DirectX::XMFLOAT2(100, 100));
    }

    // �G�t�F�N�g�ǂݍ���
    effect[0] = new Effect("./resources/effect/box.efk");
    effect[1] = new Effect("./resources/effect/ring.efk");

    effect[0]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.6f));
}

// �I��
void StageTutorial::Finalize()
{
    // enemy�I����
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear(); // vector�N���A

    // player�I����
    PlayerManager::Instance().Finalize();

    // trttain�I����
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear(); // vector�N���A

    // uimanager
    UIManager::Instance().Finalize();
    UIManager::Instance().Clear();

    effect[0]->Stop(effect[0]->handle);
    effect[1]->Stop(effect[1]->handle);
    delete effect[0];
    delete effect[1];
}

// Update�̑O�ɌĂ΂��
void StageTutorial::Begin()
{
    // terrain
    TerrainManager::Instance().Begin();

    // player
    PlayerManager::Instance().Begin();

    // enemy
    EnemyManager::Instance().Begin();
}

// �X�V����
void StageTutorial::Update(const float& elapsedTime)
{
    // terrain�X�V
    TerrainManager::Instance().Update(elapsedTime);

    // player�X�V
    PlayerManager::Instance().Update(elapsedTime);

    // enemy�X�V
    EnemyManager::Instance().Update(elapsedTime);

    // ��ʒ[�Ŏ~�܂�悤�ɂ���
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        if (playerPos.x <= -10.4f)playerPos.x = -10.4f;

        PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(playerPos);
    }

    // UI
    UIManager::Instance().Update(elapsedTime);

    // tutorialstate
    TutorialStateUpdate(elapsedTime);
}

// Update�̌�ɌĂ΂��
void StageTutorial::End()
{
    // terrain
    TerrainManager::Instance().End();

    // player
    PlayerManager::Instance().End();

    // enemy
    EnemyManager::Instance().End();
}

// �`�揈��
void StageTutorial::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // terrain
    TerrainManager::Instance().Render(elapsedTime);

    // player
    PlayerManager::Instance().Render(elapsedTime);

    // enemy
    EnemyManager::Instance().Render(elapsedTime);

    // �w�i��
    back->Render(elapsedTime);

    // �Ŕ�
    signboard->Render(elapsedTime);

    TutorialStateRender(elapsedTime);

    // UI
    {
        shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);

        UIManager::Instance().Render(elapsedTime);
    }
}

// debug�p
void StageTutorial::DrawDebug()
{
#ifdef USE_IMGUI

    // terrain
    TerrainManager::Instance().DrawDebug();

    // player
    PlayerManager::Instance().DrawDebug();

    // enemy
    EnemyManager::Instance().DrawDebug();

    // �w�i��
    back->DrawDebug();

    // �Ŕ�
    signboard->DrawDebug();

    // ui
    UIManager::Instance().DrawDebug();

    // effect
    effect[0]->DrawDebug();
    effect[1]->DrawDebug();

    ImGui::Begin("s");
    ImGui::DragFloat("stickTimer", &stickTime);
    ImGui::DragInt("stickState", &stickMoveState);
    ImGui::End();
#endif
}

// �`���[�g���A���i�s������
void StageTutorial::TutorialStateUpdate(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    switch (tutorialState)
    {
    case STATE::MoveReception:
        // UI
        StickState(elapsedTime);

        // effect(�΂͈̔�)
        //if (effect->GetTimer() > 4.7f)
        if (effect[0]->GetTimer() > 3.0f)
        {
            effect[0]->handle = effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 110.0f);
            //handle = effect->Play(effect->GetPosition(), effect->GetScale(), effect->GetColor());
            effect[0]->SetTimer(0.0f);
        }
        effect[0]->AddTimer(elapsedTime);

        // ���ɍs������
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        if (playerPos.x > -3.8f)
        {
            UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
            UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
            UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(false);
            UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(false);

            effect[0]->Stop(effect[0]->handle);
            effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 200.0f);

            tutorialState = STATE::JumpReception;
        }

        break;
    case STATE::JumpReception:

        // effect(��ɏ��X�ɏオ��)
        if (effect[1]->GetTimer() > 0.5f)
        {
            DirectX::XMFLOAT3 pos = effect[1]->GetPosition();
            pos.y += 1;
            if (pos.y > 7.0f)pos.y = 3.0f;
            effect[1]->SetPosition(pos);

            effect[1]->FadeOutEffect(effect[1]->GetPosition(), effect[1]->GetScale(), effect[1]->GetColor(), 0.0f);
            effect[1]->SetTimer(0.0f);
        }
        effect[1]->AddTimer(elapsedTime);

        if (gamePad.GetButton() & GamePad::BTN_A)
        {
            //StageManager::Instance().ChangeStage(new StagePlains);
            //effect->Stop(handle);
        }

        break;
    }
}

// �`���[�g���A���`�悷����
void StageTutorial::TutorialStateRender(float elapsedTime)
{
    switch (tutorialState)
    {
    case STATE::MoveReception:
        // UI
        UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(true);

        break;
    case STATE::JumpReception:
        // UI
        UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(true);

        break;
    }
}

void StageTutorial::StickState(float elapsedTime)
{
    stickTime += elapsedTime;

    // �X�e�B�b�N
    switch (stickMoveState)
    {
    case 0:
        UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(true);

        if (stickTime > 1.0f)
        {
            UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
            UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(true);

            stickTime = 0;
            stickMoveState = 1;
        }
        break;
    case 1:
        UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(true);
        

        if (stickTime > 1.0f)
        {
            UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(false);
            UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(true);
            
            stickTime = 0;
            stickMoveState = 2;
        }
        break;
    case 2:
        UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(true);
        
        if (stickTime > 1.0f)
        {
            UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
            UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(true);

            stickTime = 0;
            stickMoveState = 3;
        }
        break;
    case 3:
        UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(true);
        
        if (stickTime > 1.0f)
        {
            UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(false);
            UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(true);

            stickTime = 0;
            stickMoveState = 0;
        }
        break;
    }
}
