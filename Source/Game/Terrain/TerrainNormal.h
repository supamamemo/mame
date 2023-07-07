#pragma once

#include "Terrain.h"

class TerrainNormal : public Terrain
{
public:
    TerrainNormal(const char* const filename);
    ~TerrainNormal() override;

    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render(const float& elapsedTime)   override; // �`�揈��
    void DrawDebug()                        override;
};

