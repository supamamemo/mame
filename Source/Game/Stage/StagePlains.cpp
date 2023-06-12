#include "StagePlains.h"

// コンストラクタ
StagePlains::StagePlains()
{
    // player生成
    player = std::make_unique<Player>();

    // 草ブロック生成
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block = std::make_unique<GrassBlock>();
    }
}

// 初期化
void StagePlains::Initialize()
{
    // player初期化
    player->Initialize();

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
    player->Finalize();

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
    player->Begin();

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Begin();
    }    
}

// 更新処理
void StagePlains::Update()
{
    // player更新
    player->Update();

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
    player->End();

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->End();
    }
}

// 描画処理
void StagePlains::Render(float elapsedTime)
{
    // player
    player->Render(elapsedTime);

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
    player->DrawDebug();

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->DrawDebug();
    }

#endif
}
