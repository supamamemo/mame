#include "StagePlains.h"

// コンストラクタ
StagePlains::StagePlains()
{
    // player生成
    player = std::make_unique<Player>();

    // 草ブロック生成
    grassBlock = std::make_unique<GrassBlock>();
}

// 初期化
void StagePlains::Initialize()
{
    // player初期化
    player->Initialize();

    // 草ブロック初期化
    grassBlock->Initialize();
}

// 終了化
void StagePlains::Finalize()
{
    // player終了化
    player->Finalize();

    // 草ブロック終了化
    grassBlock->Finalize();
}

// Updateの前に呼ばれる処理
void StagePlains::Begin()
{
    // player
    player->Begin();

    // 草ブロック
    grassBlock->Begin();
}

// 更新処理
void StagePlains::Update()
{
    // player更新
    player->Update();

    // 草ブロック更新
    grassBlock->Update();
}

// Updateの後に呼ばれる処理
void StagePlains::End()
{
    // player
    player->End();

    // 草ブロック
    grassBlock->End();
}

// 描画処理
void StagePlains::Render(float elapsedTime)
{
    // player
    player->Render(elapsedTime);

    // 草ブロック
    grassBlock->Render(elapsedTime);
}

// debug用
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    player->DrawDebug();

    // 草ブロック
    grassBlock->DrawDebug();

#endif
}
