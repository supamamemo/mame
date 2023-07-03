#pragma once
#include "Enemy.h"

// �����A�j���[�V����
enum TofuAnimation
{
    Walk,
    Turn,
};

// �����N
class EnemyTofu : public Enemy
{
public:
    EnemyTofu();
    ~EnemyTofu() override;

    void Initialize()                      override; // ������
    void Finalize()                        override; // �I����
    void Begin()                           override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)  override; // �X�V����
    void End()                             override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime)  override; // �`�揈��
    void DrawDebug()                       override; // �f�o�b�O�`��

    void OnLanding()                       override; // ���n���ɌĂ΂��
    void OnDamaged()                       override; // �_���[�W���󂯂��Ƃ��ɌĂ΂��
    void OnDead()                          override; // ���S�����Ƃ��ɌĂ΂��
    void OnFallDead()                      override; // �����~�X�����Ƃ��ɌĂ΂��

    void OnHitWall()                       override; // �ǂɓ����������ɌĂ΂��
    void OnAttacked()                      override; // �U�������Ƃ��ɌĂ΂��
    void OnHitFriend()                     override; // �����ɓ����������ɌĂ΂��

public:
    // Imgui���O���h�~�̂��߂̂�� //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui���O���h�~�̂��߂̂�� //

private:
    std::string name = {};

};

