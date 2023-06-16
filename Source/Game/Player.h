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
    void Update(const float& elapsedTime);                          // �X�V����
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

private: // �ϐ��֘A
    State   state           = State::Idle; // ���݂̃X�e�[�g



    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders;
};

