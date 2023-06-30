#include "StagePlains.h"

#include <sstream>

#include "../../Mame/Graphics/Camera.h"
#include "../EnemyManager.h"
#include "../EnemyCannon.h"
#include "../Terrain/TerrainManager.h"
#include "../Terrain/TerrainPlains.h"

// コンストラクタ
StagePlains::StagePlains()
{
    // terrain生成
    //const int terrainCount = 26;
    for (int i = 1; i <= 15; ++i)
    {
        NO_CONST std::ostringstream ss = {};
        ss << "./resources/stage/" << i << ".fbx";
        TerrainManager::Instance().Register(new TerrainPlains(ss.str().c_str()));
    }    
    for (int i = 15; i <= 21; ++i)
    {
        NO_CONST std::ostringstream ss = {};
        ss << "./resources/stage/" << i << ".fbx";
        TerrainManager::Instance().Register(new TerrainPlains(ss.str().c_str()));
    }
    TerrainManager::Instance().Register(new TerrainPlains("./resources/stage/15.fbx"));
    TerrainManager::Instance().Register(new TerrainPlains("./resources/stage/15.fbx"));
    TerrainManager::Instance().Register(new TerrainPlains("./resources/stage/22.fbx"));
    TerrainManager::Instance().Register(new TerrainPlains("./resources/stage/23.fbx"));


    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // enemy初期化
    {
        EnemyCannon* cannon = new EnemyCannon();
        cannon->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 20.0f));
        EnemyManager::Instance().Register(cannon);

        EnemyCannon* cannon2 = new EnemyCannon();
        cannon2->GetTransform()->SetPosition(DirectX::XMFLOAT3(-5.0f, 0.0f, 10.0f));
        EnemyManager::Instance().Register(cannon2);
    }
}

// 初期化
void StagePlains::Initialize()
{
    // camera初期化
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 10, -12));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(10), 0, 0, 0));

    // terrain初期化
    TerrainManager& terrainManager = TerrainManager::Instance();
    terrainManager.Initialize();
    // ここで地面を並べています
    {
        terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(7.0f, 1.5f, 10.0f));
        terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(8.65f, 1.5f, 10.0f));
        terrainManager.GetTerrain(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(18.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(33.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(33.0f, 1.5f, 10.0f));
        terrainManager.GetTerrain(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(52.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(58.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(8)->GetTransform()->SetPosition(DirectX::XMFLOAT3(63.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(9)->GetTransform()->SetPosition(DirectX::XMFLOAT3(68.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3(82.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(86.0f, 1.8f, 10.0f));
        terrainManager.GetTerrain(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(90.7f, 1.8f, 10.0f));
        terrainManager.GetTerrain(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(95.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(99.0f, 6.0f, 10.0f));
        terrainManager.GetTerrain(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(102.0f, 7.0f, 10.0f));
        terrainManager.GetTerrain(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(105.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(116.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(118.99f, 3.46f, 10.0f));
        terrainManager.GetTerrain(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(121.25f, 3.46f, 10.0f));
        terrainManager.GetTerrain(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(151.0f, 0.0f, 10.0f));
        terrainManager.GetTerrain(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(131.06f, 1.6f, 10.0f));
        terrainManager.GetTerrain(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(138.0f, 4.0f, 10.0f));
        terrainManager.GetTerrain(23)->GetTransform()->SetPosition(DirectX::XMFLOAT3(141.0f, 5.0f, 10.0f));
        terrainManager.GetTerrain(24)->GetTransform()->SetPosition(DirectX::XMFLOAT3(146.0f, 1.6f, 10.0f));
        terrainManager.GetTerrain(25)->GetTransform()->SetPosition(DirectX::XMFLOAT3(149.3f, 1.6f, 10.0f));        
    }


    // player初期化
    PlayerManager::Instance().Initialize();

    // enemy初期化
    EnemyManager::Instance().Initialize();
}

// 終了化
void StagePlains::Finalize()
{
    // エネミー終了化
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear();

    // player終了化
    PlayerManager::Instance().Finalize();

    // terrain終了化
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear();
}

// Updateの前に呼ばれる処理
void StagePlains::Begin()
{
    // player
    PlayerManager::Instance().Begin();

    // enemy
    EnemyManager::Instance().Begin();

    // terrain
    TerrainManager::Instance().Begin();
}

// 更新処理
void StagePlains::Update(const float& elapsedTime)
{
    // camera更新
    Camera::Instance().Update(elapsedTime);

    // player更新
    PlayerManager::Instance().Update(elapsedTime);

    // enemy更新
    EnemyManager::Instance().Update(elapsedTime);

    // terrain更新
    TerrainManager::Instance().Update(elapsedTime);
}

// Updateの後に呼ばれる処理
void StagePlains::End()
{
    // player
    PlayerManager::Instance().End();

    // enemy
    EnemyManager::Instance().End();

    // terrain
    TerrainManager::Instance().End();
}

// 描画処理
void StagePlains::Render(const float& elapsedTime)
{
    // player描画
    PlayerManager::Instance().Render(elapsedTime);

    // enemy描画
    EnemyManager::Instance().Render(elapsedTime);

    // terrain描画
    //for (std::unique_ptr<GrassBlock>& block : grassBlock)
    //{
    //    // DrawCollを少なくするためにplayerから近いものだけ描画する
    //    float playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
    //    float blockPosX  = block.get()->model->GetTransform()->GetPosition().x;
    //
    //    float vecX = playerPosX - blockPosX;
    //    vecX = (vecX > 0) ? vecX : -vecX;
    //
    //    if (vecX < 40.0f) block->Render(elapsedTime);
    //}
    TerrainManager::Instance().Render(elapsedTime);
}

// debug用
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    PlayerManager::Instance().DrawDebug();

    // enemy
    EnemyManager::Instance().DrawDebug();

    // terrain
    TerrainManager::Instance().DrawDebug();

#endif
}
