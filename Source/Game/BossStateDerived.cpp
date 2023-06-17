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
    // �^�C�}�[��0�ɂȂ�����Attack�X�e�[�g��
    if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
    
    // ���Ԃ����炷
    SubtractTime(elapsedTime);

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
    // �^�C�}�[��0�ɂȂ�����Idle�X�e�[�g��
    if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));

    // ���Ԃ����炷
    SubtractTime(elapsedTime);

    DirectX::XMFLOAT3 pos = owner->GetTransform()->GetPosition();
    float moveSpeed = moveLeft ? -1 : 1;
    pos.x += moveSpeed * elapsedTime;
    owner->GetTransform()->SetPosition(pos);

    if (pos.x > 3)moveLeft = true;
    if (pos.x < -3)moveLeft = false;

}

// �I��
void BOSS::AttackState::Exit()
{

}
