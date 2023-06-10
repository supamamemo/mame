#include "StagePlains.h"

// コンストラクタ
StagePlains::StagePlains()
{
    // player生成
    player = std::make_unique<Player>();
}

// 初期化
void StagePlains::Initialize()
{
    // player初期化
    player->Initialize();
}

// 終了化
void StagePlains::Finalize()
{
    // player終了化
    player->Finalize();
}

// Updateの前に呼ばれる処理
void StagePlains::Begin()
{
    // player
    player->Begin();
}

// 更新処理
void StagePlains::Update()
{
    // player更新
    player->Update();
}

// Updateの後に呼ばれる処理
void StagePlains::End()
{
    // player
    player->End();
}

// 描画処理
void StagePlains::Render(float elapsedTime)
{
    // player
    player->Render(elapsedTime);
}

// debug用
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    player->DrawDebug();


#endif
}
