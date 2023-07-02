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
    Bubble,         // 吹き出し
    BubbleStick,    // 吹き出しPC
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

// コンストラクタ
StageTutorial::StageTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // ステージ生成&登録
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        
        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx"));  // 0
        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx"));  // 1
    }

    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // enemy
    EnemyTofu* tofu = new EnemyTofu();
    EnemyManager::Instance().Register(tofu);

    // 背景仮
    back = std::make_unique<Box>("./resources/back.fbx");

    // 看板
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

// 初期化
void StageTutorial::Initialize()
{
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0.0f, 0.0f, 0.0f));

    // ステージ初期設定
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(32.0f, 0.0f, 10.0f));
        terrainManager.Initialize();
    }

    // player初期化
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-8.0f, 2.0f, 10.0f));
        playerManager.Initialize();
    }

    // enemy初期化
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        enemyManager.GetEnemy(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(7.5f, 1.5f, 10.0f));
        enemyManager.Initialize();
    }

    // 背景仮
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 4.0f, 32.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 22.0f, 10.0f));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0.0f, 0.0f));

    // 看板
    signboard->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 3.0f, 10.0f));

    // 誘導用
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

    // エフェクト読み込み
    effect[0] = new Effect("./resources/effect/box.efk");
    effect[1] = new Effect("./resources/effect/ring.efk");

    effect[0]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.6f));
}

// 終了
void StageTutorial::Finalize()
{
    // enemy終了化
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear(); // vectorクリア

    // player終了化
    PlayerManager::Instance().Finalize();

    // trttain終了化
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear(); // vectorクリア

    // uimanager
    UIManager::Instance().Finalize();
    UIManager::Instance().Clear();

    effect[0]->Stop(effect[0]->handle);
    effect[1]->Stop(effect[1]->handle);
    delete effect[0];
    delete effect[1];
}

// Updateの前に呼ばれる
void StageTutorial::Begin()
{
    // terrain
    TerrainManager::Instance().Begin();

    // player
    PlayerManager::Instance().Begin();

    // enemy
    EnemyManager::Instance().Begin();
}

// 更新処理
void StageTutorial::Update(const float& elapsedTime)
{
    // terrain更新
    TerrainManager::Instance().Update(elapsedTime);

    // player更新
    PlayerManager::Instance().Update(elapsedTime);

    // enemy更新
    EnemyManager::Instance().Update(elapsedTime);

    // 画面端で止まるようにする
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

// Updateの後に呼ばれる
void StageTutorial::End()
{
    // terrain
    TerrainManager::Instance().End();

    // player
    PlayerManager::Instance().End();

    // enemy
    EnemyManager::Instance().End();
}

// 描画処理
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

    // 背景仮
    back->Render(elapsedTime);

    // 看板
    signboard->Render(elapsedTime);

    TutorialStateRender(elapsedTime);

    // UI
    {
        shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);

        UIManager::Instance().Render(elapsedTime);
    }
}

// debug用
void StageTutorial::DrawDebug()
{
#ifdef USE_IMGUI

    // terrain
    TerrainManager::Instance().DrawDebug();

    // player
    PlayerManager::Instance().DrawDebug();

    // enemy
    EnemyManager::Instance().DrawDebug();

    // 背景仮
    back->DrawDebug();

    // 看板
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

// チュートリアル進行するやつ
void StageTutorial::TutorialStateUpdate(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    switch (tutorialState)
    {
    case STATE::MoveReception:
        // UI
        StickState(elapsedTime);

        // effect(緑の範囲)
        //if (effect->GetTimer() > 4.7f)
        if (effect[0]->GetTimer() > 3.0f)
        {
            effect[0]->handle = effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 110.0f);
            //handle = effect->Play(effect->GetPosition(), effect->GetScale(), effect->GetColor());
            effect[0]->SetTimer(0.0f);
        }
        effect[0]->AddTimer(elapsedTime);

        // 次に行く条件
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

        // effect(上に徐々に上がる)
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

// チュートリアル描画するやつ
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

    // スティック
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
