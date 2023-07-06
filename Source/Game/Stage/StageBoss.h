#pragma once

#include "Stage.h"

#include "../PlayerManager.h"
#include "../Boss.h"
#include "../Box.h"

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

    
    std::unique_ptr<Box> back;


    std::unique_ptr<EnemyTofu> tofu;

    std::unique_ptr<Sprite> chefHat = nullptr;

    struct SpriteStruct
    {
        DirectX::XMFLOAT2 pos{ 440,10 };
        DirectX::XMFLOAT2 texPos{ 100,100 };
    }spr;

public:
    // UI関連（急ぎで作ってるからコメントなしでごめん。。）
    void PlayerHpUiUpdate(float elapsedTime);    // プレイヤーhpUI管理
    void UpdateUi(int uiCount, float speed, int state, float elapsedTime);

    int uiState = -1;
    void SetUiState() { uiState = 0; }

    DirectX::XMFLOAT3 GetPlayerUiPosition() { return playerUiPos; }
    void SetPlayerUiPosition(DirectX::XMFLOAT3 pos) { playerUiPos = pos; }
    DirectX::XMFLOAT2 GetPlayerUiSize() { return playerUiSize; }
    void SetPlayerUiSize(DirectX::XMFLOAT2 size) { playerUiSize = size; }

    DirectX::XMFLOAT3 playerUiPos = { 10, 10 ,10 };
    DirectX::XMFLOAT2 playerUiSize = { 344, 160 };

    void subtractUiTimer(float time) { uiTimer -= time; }
    void SetUiTimer(float time) { uiTimer = time; }
    float GetUiTimer() { return uiTimer; }
    float uiTimer = 0.0f;

    DirectX::XMFLOAT3 displayUiPosition{};

private:
    enum UISPRITE
    {
        BaseMameHp,
        mameHpLeft,
        mameHpCenter,
        mameHpRight,
    };

};

