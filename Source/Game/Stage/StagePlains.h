#pragma once

#include "Stage.h"

#include "../PlayerManager.h"
#include "../GrassBlock.h"



class StagePlains : public Stage
{
public:
    StagePlains();
    ~StagePlains() override {}
    
    void Initialize()                       override;   // 初期化
    void Finalize()                         override;   // 終了処理
    void Begin()                            override;   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)   override;   // 更新処理
    void End()                              override;   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime)   override;   // 描画処理
    void DrawDebug()                        override;   // デバッグ描画

public:
    void PlayerHpUiUpdate(float elapsedTime);    // プレイヤーhpUI管理
    void UpdateUi(int uiCount, float speed, int state);

    int uiState = -1;
    void SetUiState() { uiState = 0; }

    DirectX::XMFLOAT2 GetPlayerUiPosition() { return playerUiPos; }
    void SetPlayerUiPosition(DirectX::XMFLOAT2 pos) { playerUiPos = pos; }
    DirectX::XMFLOAT2 GetPlayerUiSize() { return playerUiSize; }
    void SetPlayerUiSize(DirectX::XMFLOAT2 size) { playerUiSize = size; }

    DirectX::XMFLOAT2 playerUiPos = { 10, 10 };
    DirectX::XMFLOAT2 playerUiSize = { 344, 160 };

private:
    enum UISPRITE
    {
        BaseMameHp,
        mameHpLeft,
        mameHpCenter,
        mameHpRight,
    };
};

