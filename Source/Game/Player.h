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

private: // �u�ԓI�ɌĂ΂��֐��֘A
    void OnLanding() override;  // ���n�����Ƃ��ɌĂ΂��   
    void OnBounce()  override;  // �o�E���X����Ƃ��ɌĂ΂��
    void OnDamaged() override;  // �_���[�W���󂯂��Ƃ��ɌĂ΂��
    void OnDead()    override;  // ���S�����Ƃ��ɌĂ΂��

private: // �X�e�[�g�֐��֘A
    void TransitionIdleState();                     // �ҋ@�X�e�[�g�֑J��
    void UpdateIdleState(const float& elapsedTime); // �ҋ@�X�e�[�g�X�V����
     
    void TransitionMoveState();                     // �ړ��X�e�[�g�֑J��   
    void UpdateMoveState(const float& elapsedTime); // �ړ��X�e�[�g�X�V����
 
    void TransitionJumpState();                     // �W�����v�X�e�[�g�֑J��   
    void UpdateJumpState(const float& elapsedTime); // �W�����v�X�e�[�g�X�V����    
    
    void TransitionHipDropState();                     // �q�b�v�h���b�v�X�e�[�g�֑J��   
    void UpdateHipDropState(const float& elapsedTime); // �q�b�v�h���b�v�X�e�[�g�X�V����

private: // enum�֘A
    // �X�e�[�g
    enum class State
    {
        Idle,    // �ҋ@
        Move,    // �ړ�
        Jump,    // �W�����v
        HipDrop, // �q�b�v�h���b�v
    };

private: // �����l�֘A

private: // �ϐ��֘A
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders = {};

<<<<<<< HEAD


    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders;
=======
    DirectX::XMFLOAT3 translation   = { 0, 0, 0 };
    DirectX::XMFLOAT3 scaling       = { 1, 1, 1 };
    DirectX::XMFLOAT3 rotation      = { 0, 0, 0 };

    State   state                   = State::Idle;          // ���݂̃X�e�[�g

    float   hipDropGravity          = -3.0f;                // �q�b�v�h���b�v���̏d��

    float   defaultBounceSpeedX     =  15.0f;               // �o�E���XX���x�����l
    float   defaultBounceSpeedY     =  10.0f;               // �o�E���XY���x�����l
    float   bounceSpeedX            =  defaultBounceSpeedX; // �o�E���XX���x
    float   bounceSpeedY            =  defaultBounceSpeedY; // �o�E���XY���x
    float   bounceScaleX            =  0.75f;               // �o�E���XX���x�ɂ�����l
    float   bounceScaleY            =  0.75f;               // �o�E���XY���x�ɂ�����l

    float   saveMoveVec_n           =  0.0f;                // �v���C���[�̑O�����̒P�ʃx�N�g����ۑ�����i�o�E���X���Ɏg����j

    int     bounceCount             =  0;                   // �o�E���X��
    int     bounceLimit             =  3;                   // �ő�o�E���X��

    int     animationIndex          =  0;
>>>>>>> origin/Taki
};

