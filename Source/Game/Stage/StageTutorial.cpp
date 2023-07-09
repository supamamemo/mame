#include "StageTutorial.h"

#include "../../Mame/Graphics/Graphics.h"
#include "../../Mame/Input/Input.h"
#include "../../Mame/AudioManager.h"

#include "../PlayerManager.h"

#include "../EnemyTofu.h"
#include "../EnemyManager.h"

#include "../Terrain/TerrainBoss.h"
#include "../Terrain/TerrainNormal.h"
#include "../Terrain/TerrainNoCollision.h"
#include "../Terrain/TerrainManager.h"

#include "../UIManager.h"

#include "../Stage/StageManager.h"
#include "../Stage/StagePlains.h"

#include "../../Mame/Scene/SceneManager.h"



// コンストラクタ
StageTutorial::StageTutorial()
{
}

StageTutorial::~StageTutorial()
{

}

// 初期化
void StageTutorial::Initialize()
{
    // 生成
    {
        // ステージ生成&登録
        {
            TerrainManager& terrainManager = TerrainManager::Instance();

            terrainManager.Register(new TerrainNormal("./resources/stage/1.fbx"));  // 0
            terrainManager.Register(new TerrainNormal("./resources/stage/1.fbx"));  // 1
            terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 2
            terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 3
            terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 3
            terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 3
            terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 3
            terrainManager.Register(new TerrainNormal("./resources/stage/1.fbx"));  // 1
            terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 1

            terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_yazi.fbx")); // 看板
            terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_drop.fbx")); // 看板
            terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_drop.fbx")); // 看板
            terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_dash.fbx")); // 看板
            terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_nomal.fbx")); // 看板
            terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_nomal.fbx")); // 看板

            terrainManager.Register(new TerrainNoCollision("./resources/stage/goal.fbx")); // ゴール
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

            EnemyManager::Instance().Register(new EnemyTofu());
            EnemyManager::Instance().Register(new EnemyTofu());
            EnemyManager::Instance().Register(new EnemyTofu());
        }


        // 背景仮
        back = std::make_unique<Box>("./resources/tutorialBack.fbx");
        //back = std::make_unique<Box>("./resources/back.fbx");

        // animation
        spriteAnimation[0] = std::make_unique<SpriteAnimation>(L"./resources/tutorial/jump.png");
        spriteAnimation[1] = std::make_unique<SpriteAnimation>(L"./resources/tutorial/hipDrop.png");
        isDisplaySignBoard[0] = false;
        isDisplaySignBoard[1] = false;

        // UI
        {
            UIManager& uiManager = UIManager::Instance();

            uiManager.Register(new UI(L"./resources/tutorial/hukidasi.png"));           // hukidasi
            uiManager.Register(new UI(L"./resources/tutorial/hukidasiBig.png"));           // hukidasi
            uiManager.Register(new UI(L"./resources/tutorial/hukidasiBigBig.png"));           // hukidasi
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


    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0.0f, 0.0f, 0.0f));

    float signBoardSize = 2.0f;

    // ステージ初期設定
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        terrainManager.GetTerrain(Zimen0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(Zimen1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(40.0f, 0.0f, 10.0f));

        terrainManager.GetTerrain(Zimen2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(1.9f, 1.0f, 10.0f));
        terrainManager.GetTerrain(Zimen2)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));

        terrainManager.GetTerrain(Zimen3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(17.5f, 0.0f, 10.0f));
        terrainManager.GetTerrain(Zimen4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(37.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(Zimen5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(42.0f, 1.0f, 10.0f));
        terrainManager.GetTerrain(Zimen6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(47.0f, 2.0f, 10.0f));
 
        terrainManager.GetTerrain(Zimen3)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
        terrainManager.GetTerrain(Zimen4)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
        terrainManager.GetTerrain(Zimen5)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
        terrainManager.GetTerrain(Zimen6)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));

        terrainManager.GetTerrain(Zimen7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(80.0, 0.0f, 10.0f));
        terrainManager.GetTerrain(Zimen8)->GetTransform()->SetPosition(DirectX::XMFLOAT3(57.0, 0.0f, 10.0f));

        terrainManager.GetTerrain(SignBoard_yazi0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 3, 10.5f));
        terrainManager.GetTerrain(SignBoard_yazi0)->GetTransform()->SetScale(DirectX::XMFLOAT3(signBoardSize, signBoardSize, signBoardSize));
        terrainManager.GetTerrain(SignBoard_yazi0)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(180), 0, 0));

        terrainManager.GetTerrain(SignBoard_drop0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(3, 6, 11));
        terrainManager.GetTerrain(SignBoard_drop0)->GetTransform()->SetScale(DirectX::XMFLOAT3(signBoardSize, signBoardSize, signBoardSize));
        terrainManager.GetTerrain(SignBoard_drop0)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(180), 0, 0));

        terrainManager.GetTerrain(SignBoard_drop1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(48, 7, 11));
        terrainManager.GetTerrain(SignBoard_drop1)->GetTransform()->SetScale(DirectX::XMFLOAT3(signBoardSize, signBoardSize, signBoardSize));
        terrainManager.GetTerrain(SignBoard_drop1)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(180), 0, 0));

        terrainManager.GetTerrain(SignBoard_dash0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(19, 5, 11));
        terrainManager.GetTerrain(SignBoard_dash0)->GetTransform()->SetScale(DirectX::XMFLOAT3(signBoardSize, signBoardSize, signBoardSize));

        terrainManager.GetTerrain(SignBoard_noTex0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, -3, 10.5f));
        terrainManager.GetTerrain(SignBoard_noTex0)->GetTransform()->SetScale(DirectX::XMFLOAT3(signBoardSize, signBoardSize, signBoardSize));

        terrainManager.GetTerrain(SignBoard_noTex0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, -3, 10.5f));
        terrainManager.GetTerrain(SignBoard_noTex0)->GetTransform()->SetScale(DirectX::XMFLOAT3(signBoardSize, signBoardSize, signBoardSize));

        terrainManager.GetTerrain(Goal)->GetTransform()->SetPosition(DirectX::XMFLOAT3(68, 6, 10));

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
        
        enemyManager.GetEnemy(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(50.0f, 1.5f, 10.0f));
        enemyManager.GetEnemy(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(52.0f, 1.5f, 10.0f));
        enemyManager.GetEnemy(7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(54.0f, 1.5f, 10.0f));
        
        enemyManager.Initialize();
    }

    // 背景仮
    //back->GetTransform()->SetPosition(DirectX::XMFLOAT3(45.0f, 7.0f, 32.0f));
    //back->GetTransform()->SetScale(DirectX::XMFLOAT3(85.0f, 50.0f, 3.0f));
    //back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(180), 0.0f, DirectX::XMConvertToRadians(180), 0.0f));
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 5.0f, 32.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 22.0f, 10.0f));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0.0f, 0.0f));

    // animation
    spriteAnimation[0]->Initialize(DirectX::XMFLOAT2(243, 145),
        DirectX::XMFLOAT2(731, 391), DirectX::XMFLOAT2(657, 360));

    spriteAnimation[1]->Initialize(DirectX::XMFLOAT2(243, 145),
        DirectX::XMFLOAT2(731, 391), DirectX::XMFLOAT2(1451, 771));


    // 誘導用
    tutorialState = STATE::MoveReception;

    isPlayerMove = true;

    // エフェクト読み込み
    effect[0] = new Effect("./resources/effect/box.efk");
    effect[1] = new Effect("./resources/effect/ring.efk");

    effect[0]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.6f));


    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::StageTutorial, true); // チュートリアルBGM再生
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
    SafeDelete(effect[0]);
    SafeDelete(effect[1]);

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopAllAudio(); // 全音楽停止
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
    if(isPlayerMove)
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

    // animation
    const int totalFrame = 11;      // フレームの枚数
    spriteAnimation[0]->PlayAnimation(elapsedTime, frameSpeed, totalFrame, true);
    spriteAnimation[1]->PlayAnimation(elapsedTime, frameSpeed, 14, true);
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

    // animation
    spriteAnimation[0]->DrawDebug();
    spriteAnimation[1]->DrawDebug();
    ImGui::Begin("animation");
    ImGui::DragFloat("frameSpeed", &frameSpeed);
    ImGui::End();

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

                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetPosition(DirectX::XMFLOAT3(160, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetPosition(DirectX::XMFLOAT3(160, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetPosition(DirectX::XMFLOAT3(160, 300, uiZ));
                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetSize(DirectX::XMFLOAT2(100, 100));
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
            // UI
            {
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(false);
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
            // UI
            {
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
            }

            effect[0]->Stop(effect[0]->handle);
            effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 200.0f);
            effect[1]->Stop(effect[1]->handle);
            effect[1]->FadeOutEffect(effect[1]->GetPosition(), effect[1]->GetScale(), effect[1]->GetColor(), 200.0f);

            // 入力を受け付けない
            isPlayerMove = false;
            signBoardState = 0;

            // 看板の位置をセット
            TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(8.7f, -5.0f, 1.5f));
            TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.0f, -5.2f, 3.0f));

            stickTime = 0;
            tutorialState = STATE::HipDorop;
        }

        break;
        
    case STATE::HipDorop:
        // effect
        effect[0]->SetPosition(DirectX::XMFLOAT3(17.5, 4.2f, 10.0f));
        effect[0]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.6f));

        // 看板の位置
        {
            DirectX::XMFLOAT3 terrainPos0 = TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex0)->GetTransform()->GetPosition();
            DirectX::XMFLOAT3 terrainPos1 = TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex1)->GetTransform()->GetPosition();
            DirectX::XMFLOAT4 terrainRot0 = TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex0)->GetTransform()->GetRotation();
            DirectX::XMFLOAT4 terrainRot1 = TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex1)->GetTransform()->GetRotation();

            switch (signBoardState)
            {
            case 0:
                // カメラが到達してから更新
                if (Camera::Instance().GetTransform()->GetPosition().x >= 9.0f)
                {
                    terrainPos0.y += elapsedTime * 10;
                    terrainPos1.y += elapsedTime * 10;
                }

                // 看板の最終位置(アニメーション流し始める)
                if (terrainPos0.y >= 5.2f)
                {
                    terrainPos0.y = 5.2f;
                    isDisplaySignBoard[0] = true;
                    signBoardState = 1;
                }

                terrainRot0 = DirectX::XMFLOAT4(DirectX::XMConvertToRadians(10), 0, 0, 0);
                terrainRot1 = DirectX::XMFLOAT4(DirectX::XMConvertToRadians(10), DirectX::XMConvertToRadians(15), 0, 0);
                break;
            case 1:
                // 入力
                {
                    if (gamePad.GetButtonDown() & GamePad::BTN_A)
                    {
                        isDisplaySignBoard[0] = false;
                        signBoardState = 5;
                    }


                    float aLx = gamePad.GetAxisLX();
                    if (gamePad.GetButton() & GamePad::BTN_RIGHT ||
                        aLx >= 0.7f)
                    {
                        signBoardState = 2;
                        isDisplaySignBoard[0] = false;
                    }
                }

                break;

            case 2:
                terrainPos0.x -= elapsedTime * 10;
                terrainPos1.x -= elapsedTime * 10;
                terrainPos0.y -= elapsedTime * 10;
                terrainPos1.y += elapsedTime * 10;
                terrainPos0.z += elapsedTime * 10;
                terrainPos1.z -= elapsedTime * 10;
                terrainRot0.y -= DirectX::XMConvertToRadians(0.1f);
                terrainRot1.y -= DirectX::XMConvertToRadians(0.1f);

                if (terrainPos0.x <= 6.7f)terrainPos0.x = 6.7f;
                if (terrainPos1.x <= 8.7f)terrainPos1.x = 8.7f;
                if (terrainPos0.y <= 5.0f)terrainPos0.y = 5.0f;
                if (terrainPos1.y >= 5.2f)terrainPos1.y = 5.2f;
                if (terrainPos0.z >= 3.0f)terrainPos0.z = 3.0f;
                if (terrainPos1.z <= 1.5f)terrainPos1.z = 1.5f;
                if (terrainRot0.y <= DirectX::XMConvertToRadians(-15))terrainRot0.y = DirectX::XMConvertToRadians(-15);
                if (terrainRot1.y <= DirectX::XMConvertToRadians(0))terrainRot1.y = DirectX::XMConvertToRadians(0);

                if(terrainPos1.x <= 8.7f)
                    isDisplaySignBoard[1] = true;

                // 入力
                {
                    float aLx = gamePad.GetAxisLX();
                    if (gamePad.GetButton() & GamePad::BTN_LEFT ||
                        aLx <= -0.7f)
                    {
                        signBoardState = 3;
                        isDisplaySignBoard[1] = false;
                    }

                    if (gamePad.GetButtonDown() & GamePad::BTN_A)
                    {
                        isDisplaySignBoard[1] = false;
                        signBoardState = 5;
                    }
                }

                break;
                
            case 3:
                terrainPos0.x += elapsedTime * 10;
                terrainPos1.x += elapsedTime * 10;
                terrainPos0.y += elapsedTime * 10;
                terrainPos1.y -= elapsedTime * 10;
                terrainPos0.z -= elapsedTime * 10;
                terrainPos1.z += elapsedTime * 10;
                terrainRot0.y += DirectX::XMConvertToRadians(0.1f);
                terrainRot1.y += DirectX::XMConvertToRadians(0.1f);

                if (terrainPos0.x >= 8.7f)terrainPos0.x = 8.7f;
                if (terrainPos1.x >= 11.0f)terrainPos1.x = 11.0f;
                if (terrainPos0.y >= 5.2f)terrainPos0.y = 5.2f;
                if (terrainPos1.y <= 5.0f)terrainPos1.y = 5.0f;
                if (terrainPos0.z <= 1.5f)terrainPos0.z = 1.5f;
                if (terrainPos1.z >= 3.0f)terrainPos1.z = 3.0f;
                if (terrainRot0.y >= DirectX::XMConvertToRadians(0))terrainRot0.y = DirectX::XMConvertToRadians(0);
                if (terrainRot1.y >= DirectX::XMConvertToRadians(15))terrainRot1.y = DirectX::XMConvertToRadians(15);

                if (terrainPos0.x >= 8.7f)
                    isDisplaySignBoard[0] = true;

                // 入力
                {
                    float aLx = gamePad.GetAxisLX();
                    if (gamePad.GetButton() & GamePad::BTN_RIGHT ||
                        aLx >= 0.7f)
                    {
                        signBoardState = 2;
                        isDisplaySignBoard[0] = false;
                    }
                }

                break;

            case 4:
                break;

            case 5:
                terrainPos0.y -= elapsedTime * 10;
                terrainPos1.y -= elapsedTime * 10;

                if (terrainPos0.y <= -10.0f)
                {
                    isPlayerMove = true;
                    terrainPos0.y = -10.0f;
                }

                break;
            }

            TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex0)->GetTransform()->SetPosition(terrainPos0);
            TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex1)->GetTransform()->SetPosition(terrainPos1);
            TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex0)->GetTransform()->SetScale(DirectX::XMFLOAT3(7.0f, 7.0f, 2.0f));
            TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex1)->GetTransform()->SetScale(DirectX::XMFLOAT3(7.0f, 7.0f, 2.0f));
            TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex0)->GetTransform()->SetRotation(terrainRot0);
            TerrainManager::Instance().GetTerrain(TutorialTerrain::SignBoard_noTex1)->GetTransform()->SetRotation(terrainRot1);
        }

        // UIの位置
        {
            if (isPlayerMove)
            {
                if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
                {
                    // stick
                    StickStateDown(elapsedTime);

                    UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetPosition(DirectX::XMFLOAT3(450, 90, 10));
                    UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetSize(DirectX::XMFLOAT2(150, 150));

                    UIManager::Instance().GetUI(UISPRITE::StickBase)->SetPosition(DirectX::XMFLOAT3(475, 115, 10));
                    UIManager::Instance().GetUI(UISPRITE::StickBase)->SetSize(DirectX::XMFLOAT2(100, 100));

                    UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetPosition(DirectX::XMFLOAT3(475, 115, 10));
                    UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetSize(DirectX::XMFLOAT2(100, 100));

                    UIManager::Instance().GetUI(UISPRITE::StickDown)->SetPosition(DirectX::XMFLOAT3(475, 115, 10));
                    UIManager::Instance().GetUI(UISPRITE::StickDown)->SetSize(DirectX::XMFLOAT2(100, 100));

                    // "A or B" の表示
                    UIManager::Instance().GetUI(UISPRITE::Bubble)->SetPosition(DirectX::XMFLOAT3(20, 100, 10));
                    UIManager::Instance().GetUI(UISPRITE::Bubble)->SetSize(DirectX::XMFLOAT2(350, 150));

                    UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetPosition(DirectX::XMFLOAT3(50, 120, 10));
                    UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetSize(DirectX::XMFLOAT2(100, 100));

                    UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetPosition(DirectX::XMFLOAT3(220, 120, 10));
                    UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetSize(DirectX::XMFLOAT2(100, 100));

                    UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetPosition(DirectX::XMFLOAT3(147, 165, 10));
                    UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetSize(DirectX::XMFLOAT2(80, 80));

                    // +
                    UIManager::Instance().GetUI(UISPRITE::Plus)->SetPosition(DirectX::XMFLOAT3(360, 130, 10));
                    UIManager::Instance().GetUI(UISPRITE::Plus)->SetSize(DirectX::XMFLOAT2(100, 100));
                }
                else
                {
                    UIManager::Instance().GetUI(UISPRITE::BubbleBig)->SetPosition(DirectX::XMFLOAT3(20, 100, uiZ));
                    UIManager::Instance().GetUI(UISPRITE::BubbleBig)->SetSize(DirectX::XMFLOAT2(500, 150));

                    UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetPosition(DirectX::XMFLOAT3(370, 120, uiZ));
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetSize(DirectX::XMFLOAT2(100, 100));

                    UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetPosition(DirectX::XMFLOAT3(60, 120, uiZ));
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetSize(DirectX::XMFLOAT2(240, 102.4f));

                    // +
                    UIManager::Instance().GetUI(UISPRITE::Plus)->SetPosition(DirectX::XMFLOAT3(285, 130, 10));
                    UIManager::Instance().GetUI(UISPRITE::Plus)->SetSize(DirectX::XMFLOAT2(100, 100));
                }
            }
            else
            {
                if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
                {
                    if (isDisplaySignBoard[0])
                    {
                        UIManager::Instance().GetUI(UISPRITE::StickBase)->SetPosition(DirectX::XMFLOAT3(1050, 430, uiZ));
                        UIManager::Instance().GetUI(UISPRITE::StickBase)->SetSize(DirectX::XMFLOAT2(100, 100));

                        UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetPosition(DirectX::XMFLOAT3(1050, 430, uiZ));
                        UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetSize(DirectX::XMFLOAT2(100, 100));

                        UIManager::Instance().GetUI(UISPRITE::StickRight)->SetPosition(DirectX::XMFLOAT3(1050, 430, uiZ));
                        UIManager::Instance().GetUI(UISPRITE::StickRight)->SetSize(DirectX::XMFLOAT2(100, 100));
                    }

                    if (isDisplaySignBoard[1])
                    {
                        UIManager::Instance().GetUI(UISPRITE::StickBase)->SetPosition(DirectX::XMFLOAT3(85, 430, uiZ));
                        UIManager::Instance().GetUI(UISPRITE::StickBase)->SetSize(DirectX::XMFLOAT2(100, 100));
                        
                        UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetPosition(DirectX::XMFLOAT3(85, 430, uiZ));
                        UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetSize(DirectX::XMFLOAT2(100, 100));
                        
                        UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetPosition(DirectX::XMFLOAT3(85, 430, uiZ));
                        UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetSize(DirectX::XMFLOAT2(100, 100));
                    }

                    UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetPosition(DirectX::XMFLOAT3(1245, 495, uiZ));
                    UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetSize(DirectX::XMFLOAT2(-200, 200));

                    UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetPosition(DirectX::XMFLOAT3(1070, 520, uiZ));
                    UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetSize(DirectX::XMFLOAT2(150, 150));
                }
                else
                {
                    UIManager::Instance().GetUI(UISPRITE::Bubble)->SetPosition(DirectX::XMFLOAT3(1280, 540, uiZ));
                    UIManager::Instance().GetUI(UISPRITE::Bubble)->SetSize(DirectX::XMFLOAT2(-350, 150));

                    UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetPosition(DirectX::XMFLOAT3(80, 430, uiZ));
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetSize(DirectX::XMFLOAT2(100, 100));

                    UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetPosition(DirectX::XMFLOAT3(1060, 430, uiZ));
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetSize(DirectX::XMFLOAT2(100, 100));

                    UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetPosition(DirectX::XMFLOAT3(985, 560, uiZ));
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetSize(DirectX::XMFLOAT2(240, 102.4f));
                }
            }
        }        
        
        // 次に行く条件
        if (playerPos.x >= 16.0f)
        {
            // UI
            {
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickDown)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::BubbleBig)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(false);
            }

            effect[0]->Stop(effect[0]->handle);
            effect[0]->FadeOutEffect(effect[0]->GetPosition(), effect[0]->GetScale(), effect[0]->GetColor(), 200.0f);
            effect[1]->SetPosition(DirectX::XMFLOAT3(21.0f, 4.5f, 10.0f));

            TerrainManager::Instance().GetTerrain(SignBoard_yazi0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-7, 3, 10.5f));
            TerrainManager::Instance().GetTerrain(SignBoard_yazi0)->GetTransform()->SetScale(DirectX::XMFLOAT3(2, 2, 2));
            TerrainManager::Instance().GetTerrain(SignBoard_yazi0)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(180), 0, 0));

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
                UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetSize(DirectX::XMFLOAT2(150, 150));

                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetPosition(DirectX::XMFLOAT3(70, 115, 10));
                UIManager::Instance().GetUI(UISPRITE::StickBase)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetPosition(DirectX::XMFLOAT3(70, 115, 10));
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetPosition(DirectX::XMFLOAT3(70, 115, 10));
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetPosition(DirectX::XMFLOAT3(70, 115, 10));
                UIManager::Instance().GetUI(UISPRITE::StickRight)->SetSize(DirectX::XMFLOAT2(100, 100));


                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetPosition(DirectX::XMFLOAT3(260, 100, 10));
                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetSize(DirectX::XMFLOAT2(350, 150));

                UIManager::Instance().GetUI(UISPRITE::GamePadX)->SetPosition(DirectX::XMFLOAT3(290, 120, 10));
                UIManager::Instance().GetUI(UISPRITE::GamePadX)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::GamePadY)->SetPosition(DirectX::XMFLOAT3(475, 120, 10));
                UIManager::Instance().GetUI(UISPRITE::GamePadY)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetPosition(DirectX::XMFLOAT3(395, 165, 10));
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetSize(DirectX::XMFLOAT2(80, 80));

                // +
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetPosition(DirectX::XMFLOAT3(185, 130, 10));
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetSize(DirectX::XMFLOAT2(100, 100));
            }
            else
            {
                UIManager::Instance().GetUI(UISPRITE::BubbleBigBig)->SetPosition(DirectX::XMFLOAT3(10, 100, uiZ));
                UIManager::Instance().GetUI(UISPRITE::BubbleBigBig)->SetSize(DirectX::XMFLOAT2(600, 150));

                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetPosition(DirectX::XMFLOAT3(45, 120, uiZ));
                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetPosition(DirectX::XMFLOAT3(165, 120, uiZ));
                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetSize(DirectX::XMFLOAT2(100, 100));

                UIManager::Instance().GetUI(UISPRITE::KeyBoardSHIFT)->SetPosition(DirectX::XMFLOAT3(340, 120, uiZ));
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSHIFT)->SetSize(DirectX::XMFLOAT2(240, 102.4f));

                UIManager::Instance().GetUI(UISPRITE::Plus)->SetPosition(DirectX::XMFLOAT3(255, 130, uiZ));
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetSize(DirectX::XMFLOAT2(100, 100));
            }
        }

        // 次に行く条件
        if (playerPos.x >= 35.0f)
        {
            // UI
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
                UIManager::Instance().GetUI(UISPRITE::BubbleBigBig)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::KeyBoardSHIFT)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(false);
            }

            Camera::Instance().SetTutorialState();
            tutorialState = STATE::Free;
        }

        break;
    case STATE::Free:
        if (playerPos.x >= 57.0f)
        {
            tutorialState = STATE::GoalState;
        }

        break;
    case STATE::GoalState:
        if (playerPos.x < 57.0f)
        {
            tutorialState = STATE::Free;
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

        // 看板アニメーション
        {
            if(isDisplaySignBoard[0])
            {
                Graphics::Instance().GetShader()->SetState(Graphics::Instance().GetDeviceContext(), 3, 0, 0);
                spriteAnimation[0]->Render();
            }
            if (isDisplaySignBoard[1])
            {
                Graphics::Instance().GetShader()->SetState(Graphics::Instance().GetDeviceContext(), 3, 0, 0);
                spriteAnimation[1]->Render();
            }
        }

        // UI
        {
            if (isPlayerMove)
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
                    UIManager::Instance().GetUI(UISPRITE::BubbleBig)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
                }
                else
                {
                    // gamepad解除
                    UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::StickDown)->SetIsRender(false);

                    UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);

                    // keyboard
                    UIManager::Instance().GetUI(UISPRITE::BubbleBig)->SetIsRender(true);
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetIsRender(true);
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(true);
                    UIManager::Instance().GetUI(UISPRITE::Plus)->SetIsRender(true);
                }


            }
            else
            {
                // 解除
                {
                    // gamepad解除
                    UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::StickDown)->SetIsRender(false);

                    UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::GamePadB)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);

                    // keyboard解除
                    UIManager::Instance().GetUI(UISPRITE::BubbleBig)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardS)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
                }

                // 登録
                if (GamePad::GAMEPAD_OR_KEYBOARD == 0)
                {
                    UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);


                    // jumpAnimation
                    if (isDisplaySignBoard[0])
                    {
                        StickAnimation(elapsedTime, 0);
                        UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(true);

                        UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                    }
                    else if(!isDisplaySignBoard[1])
                    {
                        UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(false);
                    }

                    // hipdropAnimation
                    if (isDisplaySignBoard[1])
                    {
                        StickAnimation(elapsedTime, 1);
                        UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(true);

                        UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(true);
                        UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(true);
                    }
                    else if(!isDisplaySignBoard[0])
                    {
                        UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(false);

                        UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);
                    }
                }
                else
                {
                    UIManager::Instance().GetUI(UISPRITE::StickBase)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::StickRight)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::BubbleStick)->SetIsRender(false);
                    UIManager::Instance().GetUI(UISPRITE::GamePadA)->SetIsRender(false);

                    if (isDisplaySignBoard[0])
                    {
                        UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(true);

                        UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
                    }
                    else
                    {
                        UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(false);
                    }

                    if (isDisplaySignBoard[1])
                    {
                        UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(true);
                        UIManager::Instance().GetUI(UISPRITE::KeyBoardD)->SetIsRender(false);

                        UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(true);
                        UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(true);
                    }
                    else
                    {                        
                        UIManager::Instance().GetUI(UISPRITE::KeyBoardA)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                        UIManager::Instance().GetUI(UISPRITE::KeyBoardSPACE)->SetIsRender(false);
                    }

                }
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
            effect[1]->SetRotate(DirectX::XMFLOAT3(0, ToRadian(effect[1]->GetAngle()), ToRadian(90.0f)));

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
                UIManager::Instance().GetUI(UISPRITE::BubbleBigBig)->SetIsRender(false);
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

                UIManager::Instance().GetUI(UISPRITE::Bubble)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadX)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::GamePadY)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::SpriteOR)->SetIsRender(false);


                // keyboard
                UIManager::Instance().GetUI(UISPRITE::BubbleBigBig)->SetIsRender(true);
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


void StageTutorial::StickAnimation(float elapsedTime, int state)
{
    stickTime += elapsedTime;

    if (state == 0)
    {
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
                stickMoveState = 0;
            }

            break;
        }
    }
    else
    {
        switch (stickMoveState)
        {
        case 0:
            UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(true);

            if (stickTime > 1.0f)
            {
                UIManager::Instance().GetUI(UISPRITE::StickCenter)->SetIsRender(false);
                UIManager::Instance().GetUI(UISPRITE::StickLeft)->SetIsRender(true);

                stickTime = 0;
                stickMoveState = 1;
            }

            break;
        case 1:
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
}