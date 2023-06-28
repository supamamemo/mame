#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"

#include "../EnemyManager.h"

#include "../Terrain/TerrainBoss.h"
#include "../Terrain/TerrainManager.h"

// コンストラクタ
StageBoss::StageBoss()
{
    Terrain* terrain = new TerrainBoss("./resources/stage/1.fbx");
    terrain->GetTransform()->AddPosition({ 0,0,0 });
    TerrainManager::Instance().Register(terrain);
        
    Terrain* terrain2 = new TerrainBoss("./resources/stage/1.fbx");
    terrain2->GetTransform()->AddPosition({ 10,7,0 });    
    TerrainManager::Instance().Register(terrain2);
    
    Terrain* terrain3 = new TerrainBoss("./resources/stage/1.fbx");
    terrain3->GetTransform()->AddPosition({ 15,2,0 });
    TerrainManager::Instance().Register(terrain3);

    //for (std::unique_ptr<Terrain>& temp : terrain)
    //{
    //    temp = std::make_unique<TerrainBoss>("./resources/temporary/assets_ground.fbx");
    //}

    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // boss生成
    //boss = std::make_unique<Boss>();
    Boss* boss = new Boss();
    EnemyManager::Instance().Register(boss); 

}

// 初期化
void StageBoss::Initialize()
{
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(10), 0.0f, 0.0f, 0.0f));


    TerrainManager& terrainManager = TerrainManager::Instance();
    terrainManager.Initialize();
    //terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 10));
    //terrainManager.GetTerrain(0)->GetTransform()->SetScale(DirectX::XMFLOAT3(4.0f, 1.0f, 1.0f));
/*    terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-11, 20, 10));
    terrainManager.GetTerrain(1)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0));
    terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(9.5f, 20, 10));
    terrainManager.GetTerrain(2)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0)); */   
   
    //terrain[0]->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 1, 10));
    //terrain[0]->GetTransform()->SetScale(DirectX::XMFLOAT3(4.0f, 1.0f, 1.0f));
    //terrain[1]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-11, 20, 10));
    //terrain[1]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0));
    //terrain[2]->GetTransform()->SetPosition(DirectX::XMFLOAT3(9.5f, 20, 10));
    //terrain[2]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0));
     
    // player初期化
    PlayerManager::Instance().Initialize();

    // boss初期化
    //boss->Initialize();
    EnemyManager::Instance().Initialize();
}

// 終了化
void StageBoss::Finalize()
{
    // boss終了化
    //boss->Finalize();
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear(); // vectorクリア

    // player終了化
    PlayerManager::Instance().Finalize();

    // terrain終了化
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear(); // vectorクリア
}

// Updateの前に呼ばれる
void StageBoss::Begin()
{
    // terrain
    TerrainManager::Instance().Begin();

    // player
    PlayerManager::Instance().Begin();

    // boss
    //boss->Begin();
    EnemyManager::Instance().Begin();
}

// 更新処理
void StageBoss::Update(const float& elapsedTime)
{
    // terrain更新
    TerrainManager::Instance().Update(elapsedTime);


    DirectX::XMFLOAT3 resultPos{};

    //if (Collision::IntersectAABBVsAABB(&player->aabb, &boss->aabb,  resultPos))
    //{
    //    DirectX::XMFLOAT3 pos = player->model->GetTransform()->GetPosition();
    //    pos.x += resultPos.x;
    //    pos.y += resultPos.y;
    //    player->model->GetTransform()->SetPosition(pos);
    //}

    //if (Collision::IntersectAABBVsAABB(&playerManager.GetPlayer()->aabb, &boss->aabb,  resultPos))
    //{
    //    DirectX::XMFLOAT3 pos = playerManager.GetPlayer()->model->GetTransform()->GetPosition();
    //    pos.x += resultPos.x;
    //    pos.y += resultPos.y;
    //    playerManager.GetPlayer()->model->GetTransform()->SetPosition(pos);
    //}


    // player更新
    PlayerManager& playerManager = PlayerManager::Instance();
    playerManager.Update(elapsedTime);

    // boss更新
    //boss->Update(elapsedTime);
    EnemyManager::Instance().Update(elapsedTime);
}

// Updateの後に呼ばれる処理
void StageBoss::End()
{
    // terrain
    TerrainManager::Instance().End();

    // player
    PlayerManager::Instance().End();

    // boss
    //boss->End();
    EnemyManager::Instance().End();
}

// 描画処理
void StageBoss::Render(const float& elapsedTime)
{
    // terrain
    TerrainManager::Instance().Render(elapsedTime);
    //// 仮
    //for (std::unique_ptr<Terrain>& temp : terrain)
    //{
    //    temp->Render(elapsedTime);
    //}

    // player描画
    PlayerManager::Instance().Render(elapsedTime);

    // boss描画
    //boss->Render(elapsedTime);
    EnemyManager::Instance().Render(elapsedTime);

}

// debug用
void StageBoss::DrawDebug()
{
#ifdef USE_IMGUI

    // terrain
    TerrainManager::Instance().DrawDebug();
    //for (std::unique_ptr<Terrain>& temp : terrain)
    //{
    //    temp->DrawDebug();
    //}

    // player
    PlayerManager::Instance().DrawDebug();

    // boss
    //boss->DrawDebug();
    EnemyManager::Instance().DrawDebug();

#endif
}
