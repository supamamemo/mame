#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"



// コンストラクタ
StageBoss::StageBoss()
{
    // player生成
    player = std::make_unique<Player>();

    // boss生成
    boss = std::make_unique<Boss>();


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
    player->Initialize();

    // boss初期化
    boss->Initialize();

    stage[0]->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 0, 10));
    stage[1]->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(4, 20, 10));
    stage[1]->model->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, DirectX::XMConvertToRadians(90), 0));
    stage[2]->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(-5.5f, 20, 10));
    stage[2]->model->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, DirectX::XMConvertToRadians(90), 0));
    
    //stage[1]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
}

// 終了化
void StageBoss::Finalize()
{
    // player終了化
    player->Finalize();

    // boss終了化
    boss->Finalize();
}

// Updateの前に呼ばれる
void StageBoss::Begin()
{
    // player
    player->Begin();

    // boss
    boss->Begin();
}

// 更新処理
void StageBoss::Update(const float& elapsedTime)
{
    // player更新
    player->Update(elapsedTime);

    // boss更新
    boss->Update();

    DirectX::XMFLOAT2 resultPos{};
    if (Collision::IntersectAABBVsAABB(&boss->aabb, &player->aabb,  resultPos))
    {
        DirectX::XMFLOAT3 pos = player->model->GetTransform()->GetPosition();
        pos.x += resultPos.x;
        pos.y += resultPos.y;
        player->model->GetTransform()->SetPosition(pos);
    }
}

// Updateの後に呼ばれる処理
void StageBoss::End()
{
    // player
    player->End();

    // boss
    boss->End();
}

// 描画処理
void StageBoss::Render(const float& elapsedTime)
{
    // player描画
    player->Render(elapsedTime);

    // boss描画
    boss->Render(elapsedTime);


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
    player->DrawDebug();

    // boss
    boss->DrawDebug();

    for (std::unique_ptr<Boss>& temp : stage)
    {
        temp->DrawDebug();
    }


#endif
}
