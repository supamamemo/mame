#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize() = 0;              // ������
    virtual void Finalize() = 0;                // �I����
    virtual void Begin() = 0;                   // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(float elapsedTime) = 0; // �X�V����
    virtual void End() = 0;                     // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(float elapsedTime) = 0; // �`�揈��
    virtual void DrawDebug() = 0;
};

