#pragma once

#include "Terrain.h"

class TerrainPlains : public Terrain
{
public:
    TerrainPlains(const char* const filename);
    ~TerrainPlains() override;

    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Begin()                            override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)   override; // 更新処理
    void End()                              override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime)   override; // 描画処理
    void DrawDebug()                        override;
};

