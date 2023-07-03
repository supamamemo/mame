#pragma once

#include "Character.h"

// �^�C�g���p�v���C���[
class TitlePlayer : public Character
{
public:
    TitlePlayer();
    ~TitlePlayer() override;

    void Initialize();                      // ������
    void Finalize();                        // �I����
    void Begin() {}                         // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);  // �X�V����
    void End()   {}                         // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime);  // �`�揈��

    void DrawDebug() override;              // �f�o�b�O�`��

private:
    // �~�^��
    void CircularMotion(const float elapsedTime);

    void Turn(
        const float elapsedTime, 
        NO_CONST float vx,
        NO_CONST float vz,
        NO_CONST float turnSpeed
    );
private:
    // �A�j���[�V����
    enum Animation
    {
        //Anim_Idle,      // �ҋ@
        //Anim_Dash,      // �_�b�V���i�ꎞ�I�ȋ}�����j
        Anim_Run = 2,       // ���s
        //Anim_Break,     // �u���[�L
        //Anim_JumpInit,  // �W�����v�J�n
        //Anim_Jump,      // �W�����v
        //Anim_Fall,      // ����
        //Anim_JumpEnd,   // ���n�i�W�����v�I���j
        //Anim_HipDrop,   // �q�b�v�h���b�v
        //Anim_Walk,      // ���s
        //Anim_Max,       // �A�j���[�V�����ő吔
    };

    // XMFLOAT�̃����o�ϐ���Y��Z�ŕ���킵���̂ō쐬
    struct Vector2D_XZ
    {
        float x = 0.0f;
        float z = 0.0f;
    };

    // �~
    struct Circle
    {
        Vector2D_XZ center     = { 0,0 };   // �~�̒��S
        float       radius     = 0.0f;      // �~�̔��a
        Vector2D_XZ unitCircle = { 0,0 };   // �P�ʉ~(�P�ʃx�N�g��)
    };

private:
    Circle circle = { {}, 20.0f, {} };          // �~
    float rotationY      = 0.0f;                // ��]�l(�g�����X�t�H�[���̊p�x�𒼐ڂ�����ƈړ����x���e�����󂯂Ĕ����ɂȂ��Ă��܂��̂ŕʂŗp�ӂ��Ă���)
    float rotationSpeedY = ToRadian(-45.0f);    // ��]���xY�i�ړ����x�j
};