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
    void Update(const float& elapsedTime)   override;   // �X�V����
    void Render(const float& elapsedTime)   override;   // �`�揈��
    void DrawDebug()                        override;

    void OnRiding()                         override;   // ���ꂽ�Ƃ��ɌĂ΂��

private:
    float   fallAcceleration_   = -0.025f;  // ����������
    float   fallVelocityMax_    = -25.0f;   // �ő嗎�����x
    bool    isFall_             = false;    // �������邩�ǂ���

};
