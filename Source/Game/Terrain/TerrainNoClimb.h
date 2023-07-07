#pragma once

#include "Terrain.h"

class TerrainNoClimb : public Terrain
{
public:
    TerrainNoClimb(const char* const filename);
    ~TerrainNoClimb() override;

    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Update(const float& elapsedTime)   override; // �X�V����
    void Render(const float& elapsedTime)   override; // �`�揈��
    void DrawDebug()                        override;
};

