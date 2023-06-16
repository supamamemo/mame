#include "StagePlains.h"

// コンストラクタ
StagePlains::StagePlains()
{
    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // 草ブロック生成
#if 0
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block = std::make_unique<GrassBlock>();
    }
#else
    grassBlock[0] = std::make_unique<GrassBlock>("./resources/temporary/assets_air_ground.fbx");
    grassBlock[1] = std::make_unique<GrassBlock>("./resources/temporary/assets_air_ground_move.fbx");
    grassBlock[2] = std::make_unique<GrassBlock>("./resources/temporary/assets_block.fbx");
    grassBlock[3] = std::make_unique<GrassBlock>("./resources/temporary/assets_cannon.fbx");
    grassBlock[4] = std::make_unique<GrassBlock>("./resources/temporary/assets_cannon_bullet.fbx");
    grassBlock[5] = std::make_unique<GrassBlock>("./resources/temporary/assets_flag.fbx");
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
}

// 更新処理
void StagePlains::Update(const float& elapsedTime)
{
    // player更新
    PlayerManager::Instance().GetPlayer()->Update(elapsedTime);

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

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->DrawDebug();
    }

#endif
}
