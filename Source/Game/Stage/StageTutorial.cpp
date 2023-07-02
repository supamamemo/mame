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

    // 誘導用
    box = std::make_unique<Box>("./resources/cube.fbx");

    // UI
    {
        UIManager& uiManager = UIManager::Instance();

        uiManager.Register(new UI(L"./resources/tutorial/stickBase.png"));  // 0
        uiManager.Register(new UI(L"./resources/tutorial/stick.png"));      // 1
        uiManager.Register(new UI(L"./resources/tutorial/A.png"));          // 2
    }

    // エフェクト読み込み
    effect = new Effect("./resources/effect/e/bimu.efk");
}

StageTutorial::~StageTutorial()
{
    delete effect;
}

// 初期化
void StageTutorial::Initialize()
{
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(10), 0.0f, 0.0f, 0.0f));

    // ステージ初期設定
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        terrainManager.Initialize();
        terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(32.0f, 0.0f, 10.0f));
    }

    // player初期化
    PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-8.0f, 2.0f, 10.0f));

    // enemy初期化
    EnemyManager::Instance().Initialize();
    EnemyManager::Instance().GetEnemy(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(7.5f, 1.5f, 10.0f));
    EnemyManager::Instance().GetEnemy(0)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(270), 0.0f, 0.0f));

    // 背景仮
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 4.0f, 32.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 22.0f, 10.0f));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0.0f, 0.0f));

    // 誘導用
    {
        box->GetTransform()->SetPosition(DirectX::XMFLOAT3(-3.0f, 2.5f, 10.0f));
        box->GetTransform()->SetScale(DirectX::XMFLOAT3(150.0f, 50.0f, 130.0f));
        box->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.3f));
    }

    // UI
    {
        UIManager::Instance().GetUI(0)->SetPosition(DirectX::XMFLOAT2(160, 424));
        UIManager::Instance().GetUI(0)->SetSize(DirectX::XMFLOAT2(100, 100));
        UIManager::Instance().GetUI(1)->SetPosition(DirectX::XMFLOAT2(170, 424));
        UIManager::Instance().GetUI(1)->SetSize(DirectX::XMFLOAT2(100, 100));
        UIManager::Instance().GetUI(2)->SetPosition(DirectX::XMFLOAT2(350, 330));
        UIManager::Instance().GetUI(2)->SetSize(DirectX::XMFLOAT2(100, 100));
    }
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

    // effect
    //effect->Play({ 0,0,0 });
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

    // 誘導用
    box->DrawDebug();

    // ui
    UIManager::Instance().DrawDebug();

    ImGui::Begin("s");
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
        UIManager::Instance().GetUI(0)->SetIsRender(true);
        UIManager::Instance().GetUI(1)->SetIsRender(true);
        StickState(elapsedTime);

        // 次に行く条件
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        if (playerPos.x > -3.8f)
        {
            UIManager::Instance().GetUI(0)->SetIsRender(false);
            UIManager::Instance().GetUI(1)->SetIsRender(false);

            tutorialState = STATE::JumpReception;
        }

        break;
    case STATE::JumpReception:
        // UI
        UIManager::Instance().GetUI(2)->SetIsRender(true);

        break;
    }
}

// チュートリアル描画するやつ
void StageTutorial::TutorialStateRender(float elapsedTime)
{
    switch (tutorialState)
    {
    case STATE::MoveReception:
        // 誘導用
        box->Render(elapsedTime);


        break;
    case STATE::JumpReception:
        break;
    }
}

void StageTutorial::StickState(float elapsedTime)
{
    stickTime += elapsedTime;

    DirectX::XMFLOAT2 pos = UIManager::Instance().GetUI(1)->GetPosition();

    // スティック
    switch (stickMoveState)
    {
    case 0:
        pos = { 160, 424 };

        if (stickTime > 1.0f)
        {
            stickTime = 0;
            stickMoveState = 1;
        }
        break;
    case 1:
        pos = { 140,424 };

        if (stickTime > 1.0f)
        {
            stickTime = 0;
            stickMoveState = 2;
        }
        break;
    case 2:
        pos = { 160, 424 };

        if (stickTime > 1.0f)
        {
            stickTime = 0;
            stickMoveState = 3;
        }
        break;
    case 3:
        pos = { 180, 424 };

        if (stickTime > 1.0f)
        {
            stickTime = 0;
            stickMoveState = 0;
        }
        break;
    }

    UIManager::Instance().GetUI(1)->SetPosition(pos);
}
