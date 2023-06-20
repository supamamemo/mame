#pragma once

#include "Character.h"


class Player : public Character
{
public:
    Player();
    ~Player() override;

    void Initialize();                      // ������
    void Finalize();                        // �I����
    void Begin();                           // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);  // �X�V����
    void End();                             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime);  // �`�揈��

    void DrawDebug() override;              // �f�o�b�O�`��

private: // ���͏����֐��֘A   
    const float GetMoveVecX() const;                 // ���X�e�B�b�N���͒l����ړ�X�����x�N�g�����擾   
    const float GetMoveVecY() const;                 // ���X�e�B�b�N���͒l����ړ�Y�����x�N�g�����擾   
    const bool  InputMove(const float& elapsedTime); // �ړ����͏���   
    const bool  InputJump();                         // �W�����v���͏���

private: // �X�V�����֐��֘A
    void  UpdateDashCoolTimer(const float& elapsedTime); // �_�b�V���N�[���^�C���X�V����

private: // �u�ԓI�ɌĂ΂��֐��֘A
    void OnLanding() override;  // ���n�����Ƃ��ɌĂ΂��   
    void OnDash()    override;  // �_�b�V�����Ă���Ƃ��ɌĂ΂��
    void OnBounce()  override;  // �o�E���X����Ƃ��ɌĂ΂��
    void OnDamaged() override;  // �_���[�W���󂯂��Ƃ��ɌĂ΂��
    void OnDead()    override;  // ���S�����Ƃ��ɌĂ΂��

private: // �X�e�[�g�֐��֘A
#if 1
    void TransitionIdleState();                         // �ҋ@�X�e�[�g�֑J��
    void UpdateIdleState(const float& elapsedTime);     // �ҋ@�X�e�[�g�X�V����
     
    void TransitionWalkState();                         // ���s�X�e�[�g�֑J��   
    void UpdateWalkState(const float& elapsedTime);     // ���s�X�e�[�g�X�V����    
    
    void TransitionDashState();                         // �_�b�V���X�e�[�g�֑J��   
    void UpdateDashState(const float& elapsedTime);     // �_�b�V���X�e�[�g�X�V����    
    
    void TransitionRunState();                          // ���s�X�e�[�g�֑J��   
    void UpdateRunState(const float& elapsedTime);      // ���s�X�e�[�g�X�V����
 
    void TransitionJumpState();                         // �W�����v�X�e�[�g�֑J��   
    void UpdateJumpState(const float& elapsedTime);     // �W�����v�X�e�[�g�X�V����    
    
    void TransitionHipDropState();                      // �q�b�v�h���b�v�X�e�[�g�֑J��   
    void UpdateHipDropState(const float& elapsedTime);  // �q�b�v�h���b�v�X�e�[�g�X�V����
#endif
private: // enum�֘A
    // �X�e�[�g
    enum class State
    {
        Idle,    // �ҋ@
        Walk,    // ���s
        Dash,    // �_�b�V��(�ꎞ�I�ȋ}����)
        Run,     // ���s
        Jump,    // �W�����v
        HipDrop, // �q�b�v�h���b�v
    };

    // �A�j���[�V����
    enum Animation
    {
        Anim_Idle,      // �ҋ@
        Anim_Dash,      // �_�b�V���i�ꎞ�I�ȋ}�����j
        Anim_Run,       // ���s
        Anim_Break,     // ?
        Anim_JumpInit,  // �W�����v�J�n
        Anim_Jump,      // �W�����v
        Anim_Fall,      // ����
        Anim_JumpEnd,   // ���n�i�W�����v�I���j
        Anim_HipDrop,   // �q�b�v�h���b�v
        Anim_Max,       // �A�j���[�V�����ő吔
    };

private: // �ϐ��֘A
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders = {};

    State   state                   = State::Idle;          // ���݂̃X�e�[�g

    float   hipDropGravity          = -3.0f;                // �q�b�v�h���b�v���̏d��

    float   dashAcceleration        =  30.0f;               // �_�b�V������X���x
    float   defaultDashTime         =  0.1f;                // �_�b�V�����ԏ����l
    float   dashTimer               =  defaultDashTime;     // �_�b�V���^�C�}�[(�_�b�V�����̑��쐧�����Ԃ��Ǘ�����)
    float   dashFinishScale         =  0.5f;                // �_�b�V�����I����ĕʂ̃X�e�[�g�J�ڑO�ɑ��x������������l
    float   dashCoolTime            =  0.5f;
    float   dashCoolTimer           =  dashCoolTime;        // �_�b�V����A������Ȃ��悤�N�[���^�C����ݒ�

    float   runMoveSpeed            =  15.0f;               // ���s���̈ړ����x(moveSpeed�ɑ������)

    float   defaultBounceSpeedX     =  15.0f;               // �o�E���XX���x�����l
    float   defaultBounceSpeedY     =  10.0f;               // �o�E���XY���x�����l
    float   bounceSpeedX            =  defaultBounceSpeedX; // �o�E���XX���x
    float   bounceSpeedY            =  defaultBounceSpeedY; // �o�E���XY���x
    float   bounceScaleX            =  0.75f;               // �o�E���XX���x�ɂ�����l
    float   bounceScaleY            =  0.75f;               // �o�E���XY���x�ɂ�����l
    float   saveMoveVec_n           =  0.0f;                // �v���C���[�̒P�ʈړ��x�N�g����ۑ�����i�o�E���X���Ɏg����j

    int     bounceCount             =  0;                   // �o�E���X��
    int     bounceLimit             =  2;                   // �ő�o�E���X��

    bool    isUpdatedAnimation      = false;                //�@�A�j���[�V�������X�V���ꂽ��
};

