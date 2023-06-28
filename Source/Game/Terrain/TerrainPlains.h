#pragma once

#include "Terrain.h"

class TerrainPlains : public Terrain
{
public:
    TerrainPlains(const char* const filename);
    ~TerrainPlains() override;

    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Begin()                            override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override; // �X�V����
    void End()                              override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime)   override; // �`�揈��
    void DrawDebug()                        override;
};

