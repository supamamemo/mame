#include "StageTutorial.h"

#include "../../Mame/Graphics/Graphics.h"

#include "../PlayerManager.h"

#include "../EnemyTofu.h"
#include "../EnemyManager.h"

#include "../Terrain/TerrainBoss.h"
#include "../Terrain/TerrainManager.h"

// コンストラクタ
StageTutorial::StageTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // ステージ生成&登録
    {
        Terrain* terrain0 = new TerrainBoss("./resources/stage/stage.fbx");
        TerrainManager::Instance().Register(terrain0);
        Terrain* terrain1 = new TerrainBoss("./resources/stage/1.fbx");
        TerrainManager::Instance().Register(terrain1);
    }

    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // enemy
    EnemyTofu* tofu = new EnemyTofu();
    EnemyManager::Instance().Register(tofu);

    // 背景仮
    back = std::make_unique<Boss>("./resources/back.fbx");

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
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0.0f, 0.0f, 0.0f));

    // ステージ初期設定
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(32.0f, 0.0f, 10.0f));
        terrainManager.Initialize();
    }

    // player初期化
    PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-8.0f, 2.0f, 10.0f));

    // enemy初期化
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        enemyManager.GetEnemy(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(7.5f, 1.5f, 10.0f));
        enemyManager.GetEnemy(0)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(270), 0.0f, 0.0f));
        enemyManager.Initialize();
    }

    // 背景仮
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 4.0f, 32.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 22.0f, 10.0f));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0.0f, 0.0f));
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

    // effect
    effect->Play({ 0,0,0 });
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

    // UI
    {
        shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);
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

#endif
}
