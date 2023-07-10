#pragma once

#include "Terrain.h"

class TerrainDelayFall : public Terrain
{
public:
    TerrainDelayFall(const char* const filename);
    ~TerrainDelayFall() override {};

    void Initialize()                       override;   // ������
    void Update(const float& elapsedTime)   override;   // �X�V����
    void DrawDebug()                        override;   // �f�o�b�O�`��

    void OnRiding()                         override;   // ���ꂽ�Ƃ��ɌĂ΂��

private:
    float   fallAcceleration_ = -0.025f;  // ����������
    float   fallVelocityMax_  = -25.0f;   // �ő嗎�����x
    float   blinkTimer_       = 0.0f;     // �_�Ń^�C�}�[
    float   fallTimer_        = 0.0f;     // ��������܂ł̎���

    bool    isReadyFall_      = false;    // �������鏀�������邩�ǂ���
    bool    isFall_           = false;    // �������邩�ǂ���

};