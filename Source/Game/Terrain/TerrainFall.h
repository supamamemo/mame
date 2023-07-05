#pragma once

#include "Terrain.h"

// ��������n�`
class TerrainFall : public Terrain
{
public:
    TerrainFall(const char* const filename);
    ~TerrainFall() override {};

    void Initialize()                       override;   // ������
    void Finalize()                         override {} // �I����
    void Begin()                            override {} // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override;   // �X�V����
    void End()                              override {} // ���t���[����ԍŌ�ɌĂ΂��
};
