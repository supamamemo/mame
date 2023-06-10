#pragma once

#include "Stage.h"

#include "../Player.h"

class StagePlains : public Stage
{
public:
    StagePlains();
    ~StagePlains()override {}

    void Initialize()override;  // 初期化
    void Finalize()override;    // 終了処理
    void Begin()override;       // 毎フレーム一番最初に呼ばれる
    void Update()override;      // 更新処理
    void End()override;         // 毎フレーム一番最後に呼ばれる
    void Render(float elapsedTime)override;     // 描画処理
    void DrawDebug()override;

private:
    std::unique_ptr<Player>player;
};

