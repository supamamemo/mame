#pragma once

#include "Stage.h"

#include <thread>

#include "../spriteDissolve.h"
#include "../SpriteAnimation.h"

class StageLoading : public Stage
{
public:
    StageLoading(Stage* nextStage);
    ~StageLoading() override {}

    void Initialize()   override;                   // 初期化
    void Finalize()     override;                   // 終了化
    void Begin()        override;                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()          override;                   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理

    void DrawDebug()    override;

private:
    // ローディングスレッド
    static void LoadingThread(StageLoading* stage);


private:
    std::unique_ptr<SpriteDissolve> spriteDissolve = nullptr;

    std::unique_ptr<SpriteAnimation> spriteAnimation = nullptr;

    Stage* nextStage = nullptr;
    std::thread* thread = nullptr;
};

