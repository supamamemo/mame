#pragma once

#include "BaseScene.h"

#include <thread>

#include "../../Game/sprite_dissolve.h"

class SceneLoading : public Mame::Scene::BaseScene
{
public:
    SceneLoading(BaseScene* nextScene) :nextScene(nextScene) {}
    ~SceneLoading()     override {}

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
    Sprite_dissolve spriteDissolve;

    BaseScene*      nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};

