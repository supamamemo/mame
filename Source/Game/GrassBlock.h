#pragma once

#include "Block.h"

class GrassBlock :
    public Block
{
public:
    GrassBlock();
    ~GrassBlock()override;

    void Initialize();      // 初期化
    void Finalize();        // 終了化
    void Begin();           // 毎フレーム一番最初に呼ばれる
    void Update();          // 更新処理
    void End();             // 毎フレーム一番最後に呼ばれる
    void Render(float elapsedTime);          // 描画処理
    void DrawDebug()override;

    
};

