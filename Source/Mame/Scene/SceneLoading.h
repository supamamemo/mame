#pragma once

#include "BaseScene.h"

#include <thread>

#include "../../Game/spriteDissolve.h"

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

    struct Animation
    {
        DirectX::XMFLOAT2 position = {};
        DirectX::XMFLOAT2 size = {};
        DirectX::XMFLOAT2 texPos = {};
        DirectX::XMFLOAT2 texSize = {};
        int animationTime = 0;
        int animationFrame = 0;

        void PlayAnimation(const int& frameTime, const int& totalAnimationFrame);

    }anime;

    int animationSpeed = 10.0f;

private:
    std::unique_ptr<SpriteDissolve> spriteDissolve = nullptr;
    std::unique_ptr<Sprite> spriteLoadMameo = nullptr;

    BaseScene*      nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};

