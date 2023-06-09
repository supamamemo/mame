#pragma once

#include "Terrain.h"

class TerrainMoveUp : public Terrain
{
public:
    TerrainMoveUp(const char* const filename) : Terrain(filename) {}
    ~TerrainMoveUp() override {};

    void Initialize()                       override;   // 初期化
    void Update(const float& elapsedTime)   override;   // 更新処理
    void DrawDebug()                        override;   // デバッグ描画

private:
    float addSpeedY_        =  6.0f;    // スピードY
    float positionLimitY_   =  20.0f;   // 位置制限
    float resetPositionY_   = -15.0f;   // リセット位置Y
};

