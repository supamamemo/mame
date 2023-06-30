#include "StagePlains.h"

#include "../../Mame/Graphics/Camera.h"
#include "../EnemyManager.h"
#include "../EnemyTofu.h"
#include "../Terrain/TerrainManager.h"
#include "../Terrain/TerrainPlains.h"

// コンストラクタ
StagePlains::StagePlains()
{
    // terrain生成
    TerrainManager& terrainManager = TerrainManager::Instance();
    {
        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx" )); // 0
        terrainManager.Register(new TerrainPlains("./resources/stage/2.fbx" )); // 1
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx" )); // 2
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 3
        terrainManager.Register(new TerrainPlains("./resources/stage/5.fbx" )); // 4
        terrainManager.Register(new TerrainPlains("./resources/stage/6.fbx" )); // 5
        terrainManager.Register(new TerrainPlains("./resources/stage/7.fbx" )); // 6
        terrainManager.Register(new TerrainPlains("./resources/stage/8.fbx" )); // 7
        terrainManager.Register(new TerrainPlains("./resources/stage/9.fbx" )); // 8
        terrainManager.Register(new TerrainPlains("./resources/stage/10.fbx")); // 9

        terrainManager.Register(new TerrainPlains("./resources/stage/11.fbx")); // 10
        terrainManager.Register(new TerrainPlains("./resources/stage/12.fbx")); // 11
        terrainManager.Register(new TerrainPlains("./resources/stage/13.fbx")); // 12
        terrainManager.Register(new TerrainPlains("./resources/stage/14.fbx")); // 13                                                                               
        terrainManager.Register(new TerrainPlains("./resources/stage/15.fbx")); // 14
        terrainManager.Register(new TerrainPlains("./resources/stage/15.fbx")); // 15                                                                                
        terrainManager.Register(new TerrainPlains("./resources/stage/16.fbx")); // 16
        terrainManager.Register(new TerrainPlains("./resources/stage/17.fbx")); // 17
        terrainManager.Register(new TerrainPlains("./resources/stage/18.fbx")); // 18
        terrainManager.Register(new TerrainPlains("./resources/stage/19.fbx")); // 19
        terrainManager.Register(new TerrainPlains("./resources/stage/20.fbx")); // 20

        terrainManager.Register(new TerrainPlains("./resources/stage/21.fbx")); // 21
        terrainManager.Register(new TerrainPlains("./resources/stage/15.fbx")); // 22
        terrainManager.Register(new TerrainPlains("./resources/stage/15.fbx")); // 23
        terrainManager.Register(new TerrainPlains("./resources/stage/22.fbx")); // 24
        terrainManager.Register(new TerrainPlains("./resources/stage/23.fbx")); // 25
    }

    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // enemy生成
    EnemyManager& enemyManager = EnemyManager::Instance();
    {
        // 23体生成・登録
        for (int i = 0; i < 23; ++i)
        {
            enemyManager.Register(new EnemyTofu());
        }
    }
}

// 初期化
void StagePlains::Initialize()
{
    // camera初期化
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 10, -12));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0, 0, 0));


    const float posZ = 10.0f; // 奥行き

    // terrain初期化
    TerrainManager& terrainManager = TerrainManager::Instance();
    terrainManager.Initialize();
    // ここで地面を並べています
    {
        terrainManager.GetTerrain(0 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 0.0f,   0.0f,  posZ));
        terrainManager.GetTerrain(1 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 7.0f,   1.5f,  posZ));
        terrainManager.GetTerrain(2 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 8.65f,  1.5f,  posZ));
        terrainManager.GetTerrain(3 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 18.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(4 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 33.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(5 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 33.0f,  1.5f,  posZ));
        terrainManager.GetTerrain(6 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 52.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(7 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 58.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(8 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 63.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(9 )->GetTransform()->SetPosition(DirectX::XMFLOAT3( 68.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3( 82.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3( 86.0f,  1.8f,  posZ));
        terrainManager.GetTerrain(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3( 90.7f,  1.8f,  posZ));
        terrainManager.GetTerrain(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3( 95.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3( 99.0f,  6.0f,  posZ));
        terrainManager.GetTerrain(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(102.0f,  7.0f,  posZ));
        terrainManager.GetTerrain(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(105.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(116.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(118.99f, 3.46f, posZ));
        terrainManager.GetTerrain(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(121.25f, 3.46f, posZ));
        terrainManager.GetTerrain(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(151.0f,  0.0f,  posZ));
        terrainManager.GetTerrain(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(131.06f, 1.6f,  posZ));
        terrainManager.GetTerrain(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(138.0f,  4.0f,  posZ));
        terrainManager.GetTerrain(23)->GetTransform()->SetPosition(DirectX::XMFLOAT3(141.0f,  5.0f,  posZ));
        terrainManager.GetTerrain(24)->GetTransform()->SetPosition(DirectX::XMFLOAT3(146.0f,  1.6f,  posZ));
        terrainManager.GetTerrain(25)->GetTransform()->SetPosition(DirectX::XMFLOAT3(149.3f,  1.6f,  posZ));        
    }

    // player初期化
    PlayerManager& playerManager = PlayerManager::Instance();
    playerManager.Initialize();
    playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 1.5f, posZ));


    // enemy初期化
    EnemyManager& enemyManager = EnemyManager::Instance();
    enemyManager.Initialize();
    {
        enemyManager.GetEnemy(0 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.0f,   1.5f,  posZ));
        enemyManager.GetEnemy(1 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(13.0f,   1.5f,  posZ));
        enemyManager.GetEnemy(2 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(15.0f,   1.5f,  posZ));
        enemyManager.GetEnemy(3 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(24.5f,   1.5f,  posZ));
        enemyManager.GetEnemy(4 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(33.0f,   1.5f,  posZ));
        enemyManager.GetEnemy(5 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(40.0f,   1.5f,  posZ));
        enemyManager.GetEnemy(6 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(43.0f,   1.5f,  posZ));
        enemyManager.GetEnemy(7 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(52.5f,   2.0f,  posZ));
        enemyManager.GetEnemy(8 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(58.0f,   4.0f,  posZ));
        enemyManager.GetEnemy(9 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(68.0f,   3.0f,  posZ));
        enemyManager.GetEnemy(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3(75.0f,   1.75f, posZ));
        enemyManager.GetEnemy(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(81.0f,   1.75f, posZ));
        enemyManager.GetEnemy(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(88.5f,   1.75f, posZ));
        enemyManager.GetEnemy(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(112.0f,  3.35f, posZ));
        enemyManager.GetEnemy(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(114.0f,  3.35f, posZ));
        enemyManager.GetEnemy(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(116.0f,  3.35f, posZ));
        enemyManager.GetEnemy(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(121.25f, 4.5f,  posZ));
        enemyManager.GetEnemy(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(131.0f,  2.5f,  posZ));
        enemyManager.GetEnemy(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(138.0f,  1.5f,  posZ));
        enemyManager.GetEnemy(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(141.0f,  1.5f,  posZ));
        enemyManager.GetEnemy(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(153.0f,  1.5f,  posZ));
        enemyManager.GetEnemy(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(156,     1.5f,  posZ));
        enemyManager.GetEnemy(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(159,     1.5f,  posZ));
    }
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
