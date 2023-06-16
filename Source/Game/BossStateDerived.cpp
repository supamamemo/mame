#include "BossStateDerived.h"

// ������
void BOSS::IdleState::Enter()
{
    // �A�j���[�V�����Z�b�g
    owner->SetAnimation(static_cast<int>(BossAnimation::Idle));
}

// �X�V
void BOSS::IdleState::Execute(float elapsedTime)
{

}

// �I��
void BOSS::IdleState::Exit()
{

}

// ������
void BOSS::AttackState::Enter()
{
    // �A�j���[�V�����Z�b�g
    owner->SetAnimation(static_cast<int>(BossAnimation::Attack));
}

// �X�V
void BOSS::AttackState::Execute(float elapsedTime)
{

}

// �I��
void BOSS::AttackState::Exit()
{

}
