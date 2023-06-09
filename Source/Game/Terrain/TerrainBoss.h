#pragma once

#include "Terrain.h"

class TerrainBoss : public Terrain
{
public:
    TerrainBoss(const char* const filename);
    ~TerrainBoss() override;

    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render(const float& elapsedTime)   override; // 描画処理
    void DrawDebug()                        override;
};

