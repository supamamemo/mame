#pragma once

#include "Terrain.h"

class TerrainNoCollision : public Terrain
{
public:
    TerrainNoCollision(const char* const filename) : Terrain(filename) {}
    ~TerrainNoCollision() override {}

    void Initialize()                       override;   // ������
    void DrawDebug()                        override;   // �f�o�b�O�`��
};