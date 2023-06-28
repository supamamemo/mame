#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"

#include "../EnemyManager.h"

#include "../Terrain/TerrainBoss.h"
#include "../Terrain/TerrainManager.h"

// �R���X�g���N�^
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

    // player����
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // boss����
    //boss = std::make_unique<Boss>();
    Boss* boss = new Boss();
    EnemyManager::Instance().Register(boss); 

}

// ������
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
     
    // player������
    PlayerManager::Instance().Initialize();

    // boss������
    //boss->Initialize();
    EnemyManager::Instance().Initialize();
}

// �I����
void StageBoss::Finalize()
{
    // boss�I����
    //boss->Finalize();
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear(); // vector�N���A

    // player�I����
    PlayerManager::Instance().Finalize();

    // terrain�I����
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear(); // vector�N���A
}

// Update�̑O�ɌĂ΂��
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

// �X�V����
void StageBoss::Update(const float& elapsedTime)
{
    // terrain�X�V
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


    // player�X�V
    PlayerManager& playerManager = PlayerManager::Instance();
    playerManager.Update(elapsedTime);

    // boss�X�V
    //boss->Update(elapsedTime);
    EnemyManager::Instance().Update(elapsedTime);
}

// Update�̌�ɌĂ΂�鏈��
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

// �`�揈��
void StageBoss::Render(const float& elapsedTime)
{
    // terrain
    TerrainManager::Instance().Render(elapsedTime);
    //// ��
    //for (std::unique_ptr<Terrain>& temp : terrain)
    //{
    //    temp->Render(elapsedTime);
    //}

    // player�`��
    PlayerManager::Instance().Render(elapsedTime);

    // boss�`��
    //boss->Render(elapsedTime);
    EnemyManager::Instance().Render(elapsedTime);

}

// debug�p
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
