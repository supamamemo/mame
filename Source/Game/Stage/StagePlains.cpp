#include "StagePlains.h"

#include "../EnemyManager.h"
#include "../EnemyCannon.h"

// コンストラクタ
StagePlains::StagePlains()
{
    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // エネミー初期化
    //EnemyCannon* cannon = new EnemyCannon();
    //cannon->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 20.0f));
    //EnemyManager::Instance().Register(cannon);

    EnemyCannon* cannon2 = new EnemyCannon();
    cannon2->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
    EnemyManager::Instance().Register(cannon2);

    // 草ブロック生成
#if 0
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block = std::make_unique<GrassBlock>();
    }
#else
    //grassBlock[0] = std::make_unique<GrassBlock>("./resources/temporary/assets_air_ground.fbx");
    //grassBlock[1] = std::make_unique<GrassBlock>("./resources/temporary/assets_air_ground_move.fbx");
    //grassBlock[2] = std::make_unique<GrassBlock>("./resources/temporary/assets_block.fbx");
    //grassBlock[3] = std::make_unique<GrassBlock>("./resources/temporary/assets_cannon.fbx");
    //grassBlock[4] = std::make_unique<GrassBlock>("./resources/temporary/assets_cannon_bullet.fbx");
    //grassBlock[5] = std::make_unique<GrassBlock>("./resources/temporary/assets_flag.fbx");
    grassBlock[0] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[1] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[2] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[3] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[4] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[5] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[6] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
#endif
}

// 初期化
void StagePlains::Initialize()
{
    // player初期化
    PlayerManager::Instance().GetPlayer()->Initialize();

    // 草ブロック初期化
    int temp = 0;
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {        
        block->Initialize(DirectX::XMFLOAT3(0,0,0));
        temp++;
    }
}

// 終了化
void StagePlains::Finalize()
{
    // player終了化
    PlayerManager::Instance().GetPlayer()->Finalize();

    // エネミー終了化
    EnemyManager::Instance().Clear();

    // 草ブロック終了化
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Finalize();
    }
}

// Updateの前に呼ばれる処理
void StagePlains::Begin()
{
    // player
    PlayerManager::Instance().GetPlayer()->Begin();

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Begin();
    }


    grassBlock[0].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 10.0f });
    grassBlock[1].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 12.0f });
    grassBlock[2].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 14.0f });
    grassBlock[3].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 8.0f });
    grassBlock[4].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 6.0f });

}

// 更新処理
void StagePlains::Update(const float& elapsedTime)
{
    // player更新
    PlayerManager::Instance().GetPlayer()->Update(elapsedTime);

    // エネミー更新
    EnemyManager::Instance().Update(elapsedTime);

    // 草ブロック更新
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Update();
    }
}

// Updateの後に呼ばれる処理
void StagePlains::End()
{
    // player
    PlayerManager::Instance().GetPlayer()->End();

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->End();
    }
}

// 描画処理
void StagePlains::Render(const float& elapsedTime)
{
    // player
    PlayerManager::Instance().GetPlayer()->Render(elapsedTime);

    // エネミー描画
    EnemyManager::Instance().Render(elapsedTime);

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Render(elapsedTime);
    }
}

// debug用
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    PlayerManager::Instance().GetPlayer()->DrawDebug();

    // エネミー
    EnemyManager::Instance().DrawDebug();

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->DrawDebug();
    }

#endif
}
