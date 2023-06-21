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
        owner->PlayAnimation(static_cast<int>(BossAnimation::Idle), true);
                
        // materialColor��ݒ�(�Z�[�t�e�B�[(��))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 1));

        // �^�C�}�[���Z�b�g
        SetTimer(2.0f);
    }

    // �X�V
    void IdleState::Execute(float elapsedTime)
    {
        // �A�j���[�V�����X�V
        owner->UpdateAnimation(elapsedTime);

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
        owner->PlayAnimation(static_cast<int>(BossAnimation::Find), true);

        // materialColor(������(���F))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));

        // �^�C�}�[���Z�b�g
        SetTimer(2.0f);
    }

    // �X�V
    void FindState::Execute(float elapsedTime)
    {
        // �A�j���[�V�����X�V
        owner->UpdateAnimation(elapsedTime);

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
        owner->PlayAnimation(static_cast<int>(BossAnimation::Attack), true);

        // materialColor��ݒ�(�A�O���b�V�u(��))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 1));

        // position�擾
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        // ���E����
        moveSpeed = playerPos.x > ownerPos.x ? 3.0f : -3.0f;
    }

    // �X�V
    void AttackState::Execute(float elapsedTime)
    {
        // �A�j���[�V�����X�V
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        ownerPos.x += moveSpeed * elapsedTime;

        // �ǂɂԂ�������Idle�X�e�[�g��
        if (ownerPos.x > 3.3f)
        {
            ownerPos.x = 3.3f;
            owner->GetStateMachine()->SetMoveRight(false);
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Recoil));
        }
        if (ownerPos.x < -4.0f)
        {
            ownerPos.x = -4.0f;
            owner->GetStateMachine()->SetMoveRight(true);
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Recoil));
        }

        owner->GetTransform()->SetPosition(ownerPos);
    }

    // �I��
    void AttackState::Exit()
    {

    }
}

// RecoilState
namespace BOSS
{
    // ������
    void RecoilState::Enter()
    {
        // �A�j���[�V�����Z�b�g
        owner->PlayAnimation(static_cast<int>(BossAnimation::Recoil), true);

        // materialColor��ݒ�(��)
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
        
        // ����������ݒ�
        recoilCount = 0;
        
        SetRecoil(owner->GetStateMachine()->GetMoveRight() ? 1.0f : -1.0f);

    }

    // �X�V
    void RecoilState::Execute(float elapsedTime)
    {
        // �A�j���[�V�����X�V
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        float moveSpeed = owner->GetStateMachine()->GetMoveRight() ? 2.0f : -2.0f;
        ownerPos.x += moveSpeed * elapsedTime;
        recoilCount += moveSpeed * elapsedTime;

        if (owner->GetStateMachine()->GetMoveRight())
        {
            if (recoilCount > recoil)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));
        }
        else
        {
            if (recoilCount < recoil)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));
        }
        
        owner->GetTransform()->SetPosition(ownerPos);
    }

    // �I��
    void RecoilState::Exit()
    {
        recoilCount = 0;
    }
}