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

#include "../../Mame/Scene/SceneManager.h"



// コンストラクタ
StageTutorial::StageTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // ステージ生成&登録
    {
        TerrainManager& terrainManager = TerrainManager::Instance();

        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx"));  // 0
        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx"));  // 1
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx"));  // 2
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx"));  // 3
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx"));  // 3
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx"));  // 3
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx"));  // 3
        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx"));  // 1
    }

    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // enemy
    {
        EnemyManager::Instance().Register(new EnemyTofu());
        EnemyManager::Instance().Register(new EnemyTofu());
        EnemyManager::Instance().Register(new EnemyTofu());
        EnemyManager::Instance().Register(new EnemyTofu());
        EnemyManager::Instance().Register(new EnemyTofu());
    }


    // 背景仮
    back = std::make_unique<Box>("./resources/back.fbx");

    // 看板
    signboard = std::make_unique<Box>("./resources/stage/flag_drop.fbx");
    //signboard1 = std::make_unique<Box>("./resources/flag_drop_kawai.fbx");
    //signboard = std::make_unique<Box>("./resources/flag_yazi.fbx");
    //signboard = std::make_unique<Box>("./resources/frag.fbx");

    // UI
    {
        UIManager& uiManager = UIManager::Instance();

        uiManager.Register(new UI(L"./resources/tutorial/hukidasi.png"));           // hukidasi
        uiManager.Register(new UI(L"./resources/tutorial/hukidasi_stick.png"));     // hukidasiPC
        uiManager.Register(new UI(L"./resources/tutorial/stickBase.png"));          // stickbase
        uiManager.Register(new UI(L"./resources/tutorial/stick_center.png"));       // center
        uiManager.Register(new UI(L"./resources/tutorial/stick_right.png"));        // right
        uiManager.Register(new UI(L"./resources/tutorial/stick_left.png"));         // left
        uiManager.Register(new UI(L"./resources/tutorial/stick_down.png"));         // down
        uiManager.Register(new UI(L"./resources/tutorial/GamePadA.png"));       // A
        uiManager.Register(new UI(L"./resources/tutorial/GamePadB.png"));       // B
        uiManager.Register(new UI(L"./resources/tutorial/GamePadX.png"));       // X
        uiManager.Register(new UI(L"./resources/tutorial/GamePadY.png"));       // Y
        uiManager.Register(new UI(L"./resources/tutorial/PC_A.png"));           // A_PC
        uiManager.Register(new UI(L"./resources/tutorial/PC_D.png"));           // D_PC
        uiManager.Register(new UI(L"./resources/tutorial/PC_S.png"));           // S_PC
        uiManager.Register(new UI(L"./resources/tutorial/PC_shift.png"));       // SHIFT
        uiManager.Register(new UI(L"./resources/tutorial/PC_space.png"));       // SPACE
        uiManager.Register(new UI(L"./resources/tutorial/or.png"));             // or
        uiManager.Register(new UI(L"./resources/tutorial/plus.png"));             // or
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
        terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(40.0f, 0.0f, 10.0f));

        terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(1.9f, 1.0f, 10.0f));
        terrainManager.GetTerrain(2)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));

        terrainManager.GetTerrain(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(17.5f, 0.0f, 10.0f));
        terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(37.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(42.0f, 1.0f, 10.0f));
        terrainManager.GetTerrain(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(47.0f, 2.0f, 10.0f));
 
        terrainManager.GetTerrain(3)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
        terrainManager.GetTerrain(4)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
        terrainManager.GetTerrain(5)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
        terrainManager.GetTerrain(6)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));

        terrainManager.GetTerrain(7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(80.0, 0.0f, 10.0f));

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
        enemyManager.GetEnemy(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(9.5f, 1.5f, 10.0f));
        enemyManager.GetEnemy(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.5f, 1.5f, 10.0f));
        enemyManager.GetEnemy(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(13.5f, 1.5f, 10.0f));
        enemyManager.GetEnemy(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(15.5f, 1.5f, 10.0f));
        enemyManager.Initialize();
    }

    // 背景仮
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(45.0f, 7.0f, 32.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(85.0f, 50.0f, 3.0f));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(180), 0.0f, DirectX::XMConvertToRadians(180), 0.0f));

    // 看板
    signboard->GetTransform()->SetPosition(DirectX::XMFLOAT3(3.0f, 6.0f, 11.0f));
    signboard->GetTransform()->SetScale(DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f));
    signboard->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));

    //signboard1->GetTransform()->SetPosition(DirectX::XMFLOAT3(3.0f, 6.0f, 11.0f));
    //signboard1->GetTransform()->SetScale(DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f));

    // 誘導用
    tutorialState = STATE::MoveReception;


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

    // tutorialstate
    TutorialStateUpdate(elapsedTime);

    // camera
    Camera::Instance().UpdateTutorial(elapsedTime, tutorialState);

    // UI
    UIManager::Instance().Update(elapsedTime);

    // 背景
    back->BackUpdate(elapsedTime);

    //if (gamePad.GetButtonDown() & GamePad::BTN_X)
    //{
    //    //if(Mame::Scene::SceneManager::Instance().GetCurrentScene()->GetSceneType()==static_cast<int>(Mame::Scene::TYPE::GAME))
    //    //    Mame::Scene::SceneManager::Instance().GetCurrentScene()->ChangeStage(static_cast<int>(Mame::Scene::STAGE::Plains));
    //}
}

// Updateの後に呼ばれる
void StageTutorial::End()
{
    // player
    PlayerManager::Instance().End();

    // enemy
    EnemyManager::Instance().End();
}

// 描画処理
void StageTutorial::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance(); 

    // 背景仮
    back->Render(elapsedTime);

    // terrain
    TerrainManager::Instance().Render(elapsedTime);

    // player
    PlayerManager::Instance().Render(elapsedTime);

    // enemy
    EnemyManager::Instance().Render(elapsedTime);


    // 看板
    signboard->Render(elapsedTime);
    //signboard1->Render(elapsedTime);

    TutorialStateRender(elapsedTime);

    // UI
    UIManager::Instance().Render(elapsedTime);   
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
    //signboard1->DrawDebug();

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

    DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();

    float uiZ = 20.0f;

    switch (tutorialState)
    {
    case STATE::MoveReception:
        // effect
        effect[0]->SetPosition(DirectX::XMFLOAT3(-2.5f, 2.0f, 10.0f));
        effect[0]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.6f));

        // UIの位置
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                StickState(elapsedTime);

                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetPosition(DirectX::XMFLOAT3(135, 275, uiZ));
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetSize(DirectX::XMFLOAT2(150, 150));

                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetPosition(DirectX::XMFLOAT3(160, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetPosition(DirectX::XMFLOAT3(160, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetPosition(DirectX::XMFLOAT3(160, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetPosition(DirectX::XMFLOAT3(160, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::StickDown)->SetPosition(DirectX::XMFLOAT3(160, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::StickDown)->SetSize(DirectX::XMFLOAT2(100, 100));
            }
            else
            {
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetPosition(DirectX::XMFLOAT3(7, 280, uiZ));
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetSize(DirectX::XMFLOAT2(350, 150));

                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetPosition(DirectX::XMFLOAT3(60, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetPosition(DirectX::XMFLOAT3(190, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetSize(DirectX::XMFLOAT2(100, 100));
            }
        }

        // 次に行く条件
        if (playerPos.x > -3.8f)
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(false);
            }
            else
            {
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(false);
            }

            effect[0]->Stop(effect[0]->handle);
            effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 200.0f);

            stickTime = 0;
            stickMoveState = 0;
            tutorialState = STATE::JumpReception;
        }

        break;
    case STATE::JumpReception:
        // effect
        effect[0]->SetPosition(DirectX::XMFLOAT3(1.9f, 5.5f, 10.0f));
        effect[0]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.6f));

        // UIの位置
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetPosition(DirectX::XMFLOAT3(7, 280, uiZ));
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetSize(DirectX::XMFLOAT2(350, 150));

                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetPosition(DirectX::XMFLOAT3(40, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetSize(DirectX::XMFLOAT2(100, 100));
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetPosition(DirectX::XMFLOAT3(215, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetPosition(DirectX::XMFLOAT3(140, 350, uiZ));
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetSize(DirectX::XMFLOAT2(80, 80));
            }
            else
            {
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetPosition(DirectX::XMFLOAT3(7, 280, uiZ));
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetSize(DirectX::XMFLOAT2(350, 150));

                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetPosition(DirectX::XMFLOAT3(60, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetSize(DirectX::XMFLOAT2(240, 102.4f));
            }
        }

        // 次に行く条件
        if (playerPos.x >= 1.0f)
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);
            }
            else
            {
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
            }

            effect[0]->Stop(effect[0]->handle);
            effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 200.0f);
            effect[1]->Stop(effect[1]->handle);
            effect[1]->FadeOutEffect(effect[1]->GetPosition(), effect[1]->GetScale(), effect[1]->GetColor(), 200.0f);

            stickTime = 0;
            tutorialState = STATE::HipDorop;
        }

        break;
        
    case STATE::HipDorop:
        // effect
        effect[0]->SetPosition(DirectX::XMFLOAT3(17.5, 4.2f, 10.0f));
        effect[0]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.6f));


        // UIの位置
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                // stick
                StickStateDown(elapsedTime);
                
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetPosition(DirectX::XMFLOAT3(450, 90, 10));
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetPosition(DirectX::XMFLOAT3(475, 115, 10));
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetPosition(DirectX::XMFLOAT3(475, 115, 10));
                UIManager::Instance().GetUI(UISPRITE::StickDown)->SetPosition(DirectX::XMFLOAT3(475, 115, 10));

                // "A or B" の表示
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetPosition(DirectX::XMFLOAT3(20, 100, 10));
                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetPosition(DirectX::XMFLOAT3(50, 120, 10));
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetPosition(DirectX::XMFLOAT3(220, 120, 10));
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetPosition(DirectX::XMFLOAT3(147, 165, 10));

                // +
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetPosition(DirectX::XMFLOAT3(360, 130, 10));
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetSize(DirectX::XMFLOAT2(100, 100));
            }
            else
            {
                UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetPosition(DirectX::XMFLOAT3(370, 120, uiZ));
                UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetPosition(DirectX::XMFLOAT3(60, 120, uiZ));
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetSize(DirectX::XMFLOAT2(240, 102.4f));

                // +
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetPosition(DirectX::XMFLOAT3(285, 130, 10));
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetSize(DirectX::XMFLOAT2(100, 100));
            }
        }        
        
        // 次に行く条件
        if (playerPos.x >= 16.0f)
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickDown)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(false);
            }
            else
            {
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(false);
            }

            effect[0]->Stop(effect[0]->handle);
            effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 200.0f);
            effect[1]->SetPosition(DirectX::XMFLOAT3(21.0f, 4.5f, 10.0f));

            stickTime = 0;
            stickMoveState = 0;
            tutorialState = STATE::Run;
        }

        break;
    case STATE::Run:
        // effect
        effect[0]->SetPosition(DirectX::XMFLOAT3(36.8f, 4.2f, 10.0f));
        effect[0]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.6f));

        // stick
        StickState(elapsedTime);

        // UIの位置
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetPosition(DirectX::XMFLOAT3(45, 90, 10));
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetPosition(DirectX::XMFLOAT3(70, 115, 10));
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetPosition(DirectX::XMFLOAT3(70, 115, 10));
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetPosition(DirectX::XMFLOAT3(70, 115, 10));
                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetPosition(DirectX::XMFLOAT3(70, 115, 10));

                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetPosition(DirectX::XMFLOAT3(260, 100, 10));
                UIManager::Instance().GetUI(UISPRITE::GamePadX)->SetPosition(DirectX::XMFLOAT3(290, 120, 10));
                UIManager::Instance().GetUI(UISPRITE::GamePadX)->SetSize(DirectX::XMFLOAT2(100, 100));
                UIManager::Instance().GetUI(UISPRITE::GamePadY)->SetPosition(DirectX::XMFLOAT3(475, 120, 10));
                UIManager::Instance().GetUI(UISPRITE::GamePadY)->SetSize(DirectX::XMFLOAT2(100, 100));
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetPosition(DirectX::XMFLOAT3(395, 165, 10));

                // +
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetPosition(DirectX::XMFLOAT3(185, 130, 10));
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetSize(DirectX::XMFLOAT2(100, 100));
            }
            else
            {

            }
        }

        // 次に行く条件
        if (playerPos.x >= 35.0f)
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadX)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadY)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(false);
            }
            else
            {

            }

            Camera::Instance().SetTutorialState();
            tutorialState = STATE::Free;
        }

        break;
    case STATE::Free:
        break;
    }
}

// チュートリアル描画するやつ
void StageTutorial::TutorialStateRender(float elapsedTime)
{
    switch (tutorialState)
    {
    case STATE::MoveReception:
        // effect(緑の範囲)
        if (effect[0]->GetTimer() > 3.0f)
        {
            effect[0]->handle = effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 110.0f);
            effect[0]->SetTimer(0.0f);
        }
        effect[0]->AddTimer(elapsedTime);
        
        
        // UI
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                // gamepad
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(true);

                // keyboard解除
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(false);
            }
            else
            {
                // gamepad解除
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(false);

                // keyboard
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(true);
            }
        }

        break;
    case STATE::JumpReception:
        // effect(緑の範囲)
        if (effect[0]->GetTimer() > 3.0f)
        {
            effect[0]->handle = effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 110.0f);
            effect[0]->SetTimer(0.0f);
        }
        effect[0]->AddTimer(elapsedTime);

        // effect(上に徐々に上がる)
        if (effect[1]->GetTimer() > 0.5f)
        {
            DirectX::XMFLOAT3 pos = effect[1]->GetPosition();
            pos.y += 1;
            if (pos.y > 7.0f)pos.y = 3.0f;
            effect[1]->SetPosition(pos);
            

            effect[1]->handle = effect[1]->FadeOutEffect(effect[1]->GetPosition(), effect[1]->GetScale(), effect[1]->GetColor(), 0.0f);
            effect[1]->SetTimer(0.0f);
        }
        effect[1]->AddTimer(elapsedTime);

        // UI
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                // gamepad
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(true);

                // keyboard解除
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
            }
            else
            {
                // gamepad解除
                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);

                // keyboard
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(true);
            }
        }

        break;
    case STATE::HipDorop:
        // effect(緑の範囲)
        if (effect[0]->GetTimer() > 3.0f)
        {
            effect[0]->handle = effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 110.0f);
            effect[0]->SetTimer(0.0f);
        }
        effect[0]->AddTimer(elapsedTime);


        // UI
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                // gamepad
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(true);

                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(true);

                // keyboard解除
                UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
            }
            else
            {
                // gamepad解除
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickDown)->SetIsRender(false);                

                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);

                // keyboard
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(true);
            }
        }
        break;
    case STATE::Run:
        // effect(緑の範囲)
        if (effect[0]->GetTimer() > 3.0f)
        {
            effect[0]->handle = effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 110.0f);
            effect[0]->SetTimer(0.0f);
        }
        effect[0]->AddTimer(elapsedTime);

        // effect(右に徐々に移動)
        if (effect[1]->GetTimer() > 0.2f)
        {
            DirectX::XMFLOAT3 pos = effect[1]->GetPosition();
            pos.x += 1;
            pos.y = 4.5f;
            effect[1]->SubtractAngle(0.5f);
            if (pos.x > 33.0f)
            {
                pos.x = 21.0f;
                effect[1]->SetAngle(10);
            }
            effect[1]->SetPosition(pos);
            effect[1]->SetRotate(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(static_cast<int>(effect[1]->GetAngle())), DirectX::XMConvertToRadians(90)));

            effect[1]->handle = effect[1]->FadeOutEffect(effect[1]->GetPosition(), effect[1]->GetScale(), effect[1]->GetRotate(), effect[1]->GetColor(), 0.0f);
            effect[1]->SetTimer(0.0f);
        }
        effect[1]->AddTimer(elapsedTime);


        // UI
        {
            if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
            {
                // gamepad
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(true);

                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::GamePadX)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::GamePadY)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(true);

                // keyboard解除
                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSHIFT)->SetIsRender(false);
            }
            else
            {
                // gamepad解除
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(false);

                UIManager::Instance().GetUI(UISPRITE::GamePadX)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadY)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);


                // keyboard
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSHIFT)->SetIsRender(true);
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(true);
            }
        }

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

void StageTutorial::StickStateDown(float elapsedTime)
{
    stickTime += elapsedTime;

    switch (stickMoveState)
    {
    case 0:
        UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(true);

        if (stickTime > 1.0f)
        {
            UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
            UIManager::Instance().GetUI(UISPRITE::StickDown)->SetIsRender(true);

            stickTime = 0;
            stickMoveState = 1;
        }
        break;
    case 1:
            UIManager::Instance().GetUI(UISPRITE::StickDown)->SetIsRender(true);

            if (stickTime > 1.0f)
            {
                UIManager::Instance().GetUI(UISPRITE::StickDown)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(true);

                stickTime = 0;
                stickMoveState = 0;
            }

        break;
    }
}
