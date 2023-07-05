#pragma once

#include "Terrain.h"

// 落下する地形
class TerrainFall : public Terrain
{
public:
    TerrainFall(const char* const filename);
    ~TerrainFall() override {};

    void Initialize()                       override;   // 初期化
    void Finalize()                         override {} // 終了化
    void Begin()                            override {} // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)   override;   // 更新処理
    void End()                              override {} // 毎フレーム一番最後に呼ばれる
};
