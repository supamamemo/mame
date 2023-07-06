#pragma once

#include "Terrain.h"

class TerrainMoveUp : public Terrain
{
public:
    TerrainMoveUp(const char* const filename) : Terrain(filename) {}
    ~TerrainMoveUp() override {};

    void Initialize()                       override;   // ������
    void Update(const float& elapsedTime)   override;   // �X�V����
    void DrawDebug()                        override;   // �f�o�b�O�`��

private:
    float addSpeedY_        =  6.0f;    // �X�s�[�hY
    float positionLimitY_   =  20.0f;   // �ʒu����
    float resetPositionY_   = -15.0f;   // ���Z�b�g�ʒuY
};

