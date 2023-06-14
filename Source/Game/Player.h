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

    void DrawDebug() override;

private: // ���͏����֐��֘A   
    const DirectX::XMFLOAT3 GetMoveVec() const;                  // �X�e�B�b�N���͒l����ړ��x�N�g�����擾   
    const bool              InputMove(const float& elapsedTime); // �ړ����͏���   
    const bool              InputJump();                         // �W�����v���͏���

private: // �X�e�[�g�֐��֘A
    void TransitionIdleState();                     // �ҋ@�X�e�[�g�֑J��
    void UpdateIdleState(const float& elapsedTime); // �ҋ@�X�e�[�g�X�V����
     
    void TransitionMoveState();                     // �ړ��X�e�[�g�֑J��   
    void UpdateMoveState(const float& elapsedTime); // �ړ��X�e�[�g�X�V����
 
    void TransitionJumpState();                     // �W�����v�X�e�[�g�֑J��   
    void UpdateJumpState(const float& elapsedTime); // �W�����v�X�e�[�g�X�V����

private: // enum�֘A
    // �X�e�[�g
    enum class State
    {
        Idle,   // �ҋ@
        Move,   // �ړ�
        Jump,   // �W�����v
    };

private: // �ϐ��֘A
    State   state           = State::Idle; // ���݂̃X�e�[�g

    int     animationIndex  = 0;
};

