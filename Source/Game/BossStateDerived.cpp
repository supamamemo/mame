#include "Boss.h"
#include "BossStateDerived.h"

#include "PlayerManager.h"

// IdleState
namespace BOSS
{
    // ������
    void IdleState::Enter()
    {
        // �A�j���[�V�����Z�b�g
        owner->SetAnimation(static_cast<int>(BossAnimation::Idle));

        // materialColor��ݒ�(�Z�[�t�e�B�[(��))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 1));

        // �^�C�}�[���Z�b�g
        SetTimer(2.0f);
    }

    // �X�V
    void IdleState::Execute(float elapsedTime)
    {
        // �^�C�}�[��0�ɂȂ�����Finde�X�e�[�g��
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));

        // ���Ԃ����炷
        SubtractTime(elapsedTime);

    }

    // �I��
    void IdleState::Exit()
    {

    }
}

// FindState
namespace BOSS
{
    // ������
    void FindState::Enter()
    {
        // �A�j���[�V�����Z�b�g
        owner->SetAnimation(static_cast<int>(BossAnimation::Find));

        // materialColor(������(���F))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));

        // �^�C�}�[���Z�b�g
        SetTimer(2.0f);
    }

    // �X�V
    void FindState::Execute(float elapsedTime)
    {
        // �^�C�}�[��0�ɂȂ�����AttackState��
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));

        // ���Ԃ����炷
        SubtractTime(elapsedTime);
    }

    // �I��
    void FindState::Exit()
    {
    }
}

// AttackState
namespace BOSS
{
    // ������
    void AttackState::Enter()
    {
        // �A�j���[�V�����Z�b�g
        owner->SetAnimation(static_cast<int>(BossAnimation::Attack));

        // materialColor��ݒ�(�A�O���b�V�u(��))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 1));

        // position�擾
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->model->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        // ���E����
        moveSpeed = playerPos.x > ownerPos.x ? 1 : -1;
    }

    // �X�V
    void AttackState::Execute(float elapsedTime)
    {
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        ownerPos.x += moveSpeed * elapsedTime;

        // �ǂɂԂ�������Idle�X�e�[�g��
        if (ownerPos.x > 3.3f || ownerPos.x < -4.0f)
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));

        //// �^�C�}�[��0�ɂȂ�����Idle�X�e�[�g��
        //if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));

        //// ���Ԃ����炷
        //SubtractTime(elapsedTime);

        //DirectX::XMFLOAT3 pos = owner->GetTransform()->GetPosition();
        //float moveSpeed = moveLeft ? -1 : 1;
        //pos.x += moveSpeed * elapsedTime;
        //owner->GetTransform()->SetPosition(pos);

        //if (pos.x > 3)moveLeft = true;
        //if (pos.x < -3)moveLeft = false;

    }

    // �I��
    void AttackState::Exit()
    {

    }
}