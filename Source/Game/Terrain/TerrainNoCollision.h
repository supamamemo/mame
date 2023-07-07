#pragma once

#include "Terrain.h"

class TerrainNoCollision : public Terrain
{
public:
    TerrainNoCollision(const char* const filename) : Terrain(filename) {}
    ~TerrainNoCollision() override {}

    void Initialize()                       override;   // 初期化
    void DrawDebug()                        override;   // デバッグ描画
};