#pragma once
#include "Stage.h"

#include "../Boss.h"

// チュートリアルステージ
class StageTutorial : public Stage
{
public:
    StageTutorial();
    ~StageTutorial()override {}

    void Initialize()                     override;  // 初期化
    void Finalize()                       override;  // 終了処理
    void Begin()                          override;  // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override;  // 更新処理
    void End()                            override;  // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override;  // 描画処理
    void DrawDebug()                      override;  // デバッグ描画

private:
    // 仮にbossクラスで表示している
    std::unique_ptr<Boss> back = nullptr;
};

