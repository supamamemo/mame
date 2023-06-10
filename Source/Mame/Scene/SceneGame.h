#pragma once
#include "BaseScene.h"

#include "../../Game/Player.h"

class SceneGame : public Mame::Scene::BaseScene
{
public:
    SceneGame() {}
    ~SceneGame()override {}

    void Initialize()override;      // 初期化
    void Finalize()override;        // 終了化
    void Begin()override;           // 毎フレーム一番最初に呼ばれる
    void Update(float elapesdTime)override;          // 更新処理
    void End()override;             // 毎フレーム一番最後に呼ばれる
    void Render(float elapsedTime)override;          // 描画処理
    void DrawDebug()override;

public:
};

