#pragma once
#include "BaseScene.h"

#include "../../Game/spriteDissolve.h"

class SceneGame : public Mame::Scene::BaseScene
{
public:
    SceneGame();
    ~SceneGame()override {}

    void Initialize()   override;                   // 初期化
    void Finalize()     override;                   // 終了化
    void Begin()        override;                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()          override;                   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理

    void DrawDebug()    override;



public:
    // fadeOut/fadeIn用のディゾルブ
    std::unique_ptr<SpriteDissolve> spriteDissolve;

    // mameoload
    std::unique_ptr<Sprite> spriteLoadMameo = nullptr;

    // デバッグ用
    int fadeType = 1;
    int textureType = 0;

};

