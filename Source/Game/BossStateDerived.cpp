#include "Boss.h"
#include "BossStateDerived.h"


// ������
void BOSS::IdleState::Enter()
{
    // �A�j���[�V�����Z�b�g
    owner->SetAnimation(static_cast<int>(BossAnimation::Idle));

    // materialColor��ݒ�(�Z�[�t�e�B�[(��))
    owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 1));

    // �^�C�}�[���Z�b�g
    SetTimer(2.0f);
}

// �X�V
void BOSS::IdleState::Execute(float elapsedTime)
{
    // ���Ԃ����炷
    SubtractTime(elapsedTime);

    // �^�C�}�[��0�ɂȂ�����Attack�X�e�[�g��
    if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
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

    // materialColor��ݒ�(�A�O���b�V�u(��))
    owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 1));

    // �^�C�}�[���Z�b�g
    SetTimer(3.0f);
}

// �X�V
void BOSS::AttackState::Execute(float elapsedTime)
{
    // ���Ԃ����炷
    SubtractTime(elapsedTime);

    // �^�C�}�[��0�ɂȂ�����Idle�X�e�[�g��
    if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));
}

// �I��
void BOSS::AttackState::Exit()
{

}
