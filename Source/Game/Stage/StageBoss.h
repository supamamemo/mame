#pragma once

#include "Stage.h"

#include "../PlayerManager.h"
#include "../Boss.h"

#include "../CannonBall.h"


#include "../EnemyTofu.h"
#include "../Terrain/Terrain.h"

#include "../../sprite.h"

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
    //static const int TERRAIN_COUNT = 3;

    // 仮にbossクラスで表示してる
    std::unique_ptr<Boss> back;

    std::unique_ptr<EnemyTofu> tofu;

    std::unique_ptr<Sprite> chefHat = nullptr;

    struct SpriteStruct
    {
        DirectX::XMFLOAT2 pos{ 440,10 };
        DirectX::XMFLOAT2 texPos{ 100,100 };
    }spr;

};

