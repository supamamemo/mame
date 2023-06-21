#pragma once

#include "Stage.h"

#include "../PlayerManager.h"
#include "../Boss.h"

#include "../CannonBall.h"

class StageBoss : public Stage
{
public:
    StageBoss();
    ~StageBoss() override {}

    void Initialize()                     override;  // 初期化
    void Finalize()                       override;  // 終了処理
    void Begin()                          override;  // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override;  // 更新処理
    void End()                            override;  // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override;  // 描画処理
    void DrawDebug()                      override;  // デバッグ描画

private:
    std::unique_ptr<Boss> boss;

    // 仮にbossクラスで表示してる
    std::unique_ptr<Boss> stage[10];
};

