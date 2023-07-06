#pragma once

#include "Terrain.h"

// ��������n�`
class TerrainFall : public Terrain
{
public:
    TerrainFall(const char* const filename);
    ~TerrainFall() override {};

    void Initialize()                       override;   // ������
    void Update(const float& elapsedTime)   override;   // �X�V����
    void DrawDebug()                        override;   // �f�o�b�O�`��

    void OnRiding()                         override;   // ���ꂽ�Ƃ��ɌĂ΂��

private:
    float   fallAcceleration_   = -0.025f;  // ����������
    float   fallVelocityMax_    = -25.0f;   // �ő嗎�����x
    float   defaultBlinkTime_   = 1.0f;     // �_�ł��鎞��
    float   blinkTimer_         = 0.0f;     // �_�Ń^�C�}�[
    bool    isFall_             = false;    // �������邩�ǂ���

};
