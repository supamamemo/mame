#pragma once

#include "BaseScene.h"

#include <thread>

#include "../../Game/spriteDissolve.h"

#include "../../Game/SpriteAnimation.h"

class SceneLoading : public Mame::Scene::BaseScene
{
public:
    SceneLoading(BaseScene* nextScene);
    ~SceneLoading() override {}


    void Initialize()   override;                   // 初期化
    void Finalize()     override;                   // 終了化
    void Begin()        override;                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()          override;                   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理

    void DrawDebug()    override;

private:
    // ローディングスレッド
    static void LoadingThread(SceneLoading* scene);


private:
    std::unique_ptr<SpriteDissolve> spriteDissolve = nullptr;

    std::unique_ptr<SpriteAnimation> spriteAnimation[2];

    BaseScene*      nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};

