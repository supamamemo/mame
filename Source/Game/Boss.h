#pragma once

#include "Enemy.h"

enum class BossAnimation
{
    Attack,         // �U��
    Recoil,         // �ǂɓ�����
    Confusion,      // ����
    DeConfusion,    // ������������
    Idle,           // �ҋ@
    AttackBefore,   // �\������
    GetAttack,      // ���炢
    Dead,           // ����
    Fall,           // �K�����i�|���j
    Cry,            // �Ȃ�
    Walk,           // ����
};

class Boss : public Enemy
{
public:
    Boss();
    Boss(const char* filename); // ���ł�

    ~Boss() override;

    void Initialize()                     override; // ������
    void Finalize()                       override; // �I����
    void Begin()                          override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()                            override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��
    void DrawDebug()                      override; // �f�o�b�O�`��

    void OnDead()                         override; // ���S�����Ƃ��ɌĂ΂��
    void OnHitWall()                      override; // �ǂɓ����������ɌĂ΂��

    void OnDamaged()override;

public:
    // Imgui���O���h�~�̂��߂̂�� //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui���O���h�~�̂��߂̂�� //    


private:
    std::string name;   // Imgui�p
};

