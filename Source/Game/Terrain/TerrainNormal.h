#pragma once

#include "Terrain.h"

class TerrainNormal : public Terrain
{
public:
    TerrainNormal(const char* const filename);
    ~TerrainNormal() override;

    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新処理
    void Render(const float& elapsedTime)   override; // 描画処理
    void DrawDebug()                        override;
};

