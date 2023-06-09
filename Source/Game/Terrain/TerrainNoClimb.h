#pragma once

#include "Terrain.h"

class TerrainNoClimb : public Terrain
{
public:
    TerrainNoClimb(const char* const filename);
    ~TerrainNoClimb() override;

    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render(const float& elapsedTime)   override; // 描画処理
    void DrawDebug()                        override;
};

