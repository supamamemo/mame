#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"

#include "../EnemyManager.h"

// コンストラクタ
StageBoss::StageBoss()
{
    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // boss生成
    //boss = std::make_unique<Boss>();
    Boss* boss = new Boss();
    //cannon->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 20.0f));
    EnemyManager::Instance().Register(boss);

    // 仮
    for (std::unique_ptr<Boss>& temp : stage)
    {
        temp = std::make_unique<Boss>("./resources/temporary/assets_ground.fbx");
    }
}

// 初期化
void StageBoss::Initialize()
{
    // player初期化
    PlayerManager::Instance().Initialize();

    // boss初期化
    //boss->Initialize();
    EnemyManager::Instance().Initialize();

    stage[0]->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 1, 10));
    stage[0]->GetTransform()->SetScale(DirectX::XMFLOAT3(4.0f, 1.0f, 1.0f));
    stage[1]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-11, 20, 10));
    stage[1]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0));
    stage[2]->GetTransform()->SetPosition(DirectX::XMFLOAT3(9.5f, 20, 10));
    stage[2]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0));
    
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(10), 0.0f, 0.0f, 0.0f));
}

// 終了化
void StageBoss::Finalize()
{
    // player終了化
    PlayerManager::Instance().Finalize();

    // boss終了化
    //boss->Finalize();
    EnemyManager::Instance().Finalize();
}

// Updateの前に呼ばれる
void StageBoss::Begin()
{
    // player
    PlayerManager::Instance().Begin();

    // boss
    //boss->Begin();
    EnemyManager::Instance().Begin();
}

// 更新処理
void StageBoss::Update(const float& elapsedTime)
{
    DirectX::XMFLOAT3 resultPos{};

    //if (Collision::IntersectAABBVsAABB(&player->aabb, &boss->aabb,  resultPos))
    //{
    //    DirectX::XMFLOAT3 pos = player->model->GetTransform()->GetPosition();
    //    pos.x += resultPos.x;
    //    pos.y += resultPos.y;
    //    player->model->GetTransform()->SetPosition(pos);
    //}

    PlayerManager& playerManager = PlayerManager::Instance();
    //if (Collision::IntersectAABBVsAABB(&playerManager.GetPlayer()->aabb, &boss->aabb,  resultPos))
    //{
    //    DirectX::XMFLOAT3 pos = playerManager.GetPlayer()->model->GetTransform()->GetPosition();
    //    pos.x += resultPos.x;
    //    pos.y += resultPos.y;
    //    playerManager.GetPlayer()->model->GetTransform()->SetPosition(pos);
    //}


    // player更新
    playerManager.GetPlayer()->Update(elapsedTime);

    // boss更新
    //boss->Update(elapsedTime);
    EnemyManager::Instance().Update(elapsedTime);
}

// Updateの後に呼ばれる処理
void StageBoss::End()
{
    // player
    PlayerManager::Instance().End();

    // boss
    //boss->End();
    EnemyManager::Instance().End();
}

// 描画処理
void StageBoss::Render(const float& elapsedTime)
{
    // player描画
    PlayerManager::Instance().Render(elapsedTime);

    // boss描画
    //boss->Render(elapsedTime);
    EnemyManager::Instance().Render(elapsedTime);


    // 仮
    for (std::unique_ptr<Boss>& temp : stage)
    {
        temp->Render(elapsedTime);
    }
}

// debug用
void StageBoss::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    PlayerManager::Instance().DrawDebug();

    // boss
    //boss->DrawDebug();
    EnemyManager::Instance().DrawDebug();

    for (std::unique_ptr<Boss>& temp : stage)
    {
        temp->DrawDebug();
    }

#endif
}
