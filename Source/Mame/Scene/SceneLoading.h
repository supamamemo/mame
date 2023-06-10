#pragma once

#include "BaseScene.h"

#include <thread>

#include "../../Game/sprite_dissolve.h"

class SceneLoading :
    public Mame::Scene::BaseScene
{
public:
    SceneLoading(BaseScene* nextScene) :nextScene(nextScene) {}
    ~SceneLoading() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    void Begin()override;

    // 更新処理
    void Update(float elapesdTime)override;

    void End()override;

    // 描画処理
    void Render(float elapsedTime)override;

    void DrawDebug()override;

private:
    // ローディングスレッド
    static void LoadingThread(SceneLoading* scene);

private:
    Sprite_dissolve spriteDissolve;

    BaseScene* nextScene = nullptr;
    std::thread* thread = nullptr;
};

