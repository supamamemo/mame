#include "Boss.h"
#include "BossStateDerived.h"

#include "PlayerManager.h"

#include "CannonBall.h"

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
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Find));

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
        SetTimer(1.0f);

        // ��]���x�ݒ�
        SetRotationSpeed(3.0f);
    }

    // �X�V
    void FindState::Execute(float elapsedTime)
    {
        // �A�j���[�V�����X�V
        owner->UpdateAnimation(elapsedTime);

        // �^�C�}�[��0�ɂȂ�����AttackState��
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::ROTATE));

        // ���Ԃ����炷
        SubtractTime(elapsedTime);
    }

    // �I��
    void FindState::Exit()
    {
    }
}

// RotateState
namespace BOSS
{
    // ������
    void RotateState::Enter()
    {
        // materialColor(��](��))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f));

        // �v���C���[���ǂ����ɂ���̂�
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        float rot = playerPos.x > ownerPos.x ? 90.0f : -90.0f;

        owner->GetStateMachine()->SetMoveRight(true);
    }

    // �X�V
    void RotateState::Execute(float elapsedTime)
    {
        // �v���C���[�̕����Ɍ����悤�ɉ�]����

        DirectX::XMFLOAT4 rotation = owner->GetTransform()->GetRotation();
    }

    // �I��
    void RotateState::Exit()
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
        SetMoveSpeed(playerPos.x > ownerPos.x ? speed : -speed);
        float rot = playerPos.x > ownerPos.x ? 90.0f : -90.0f;
        owner->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(rot), 0.0f, 0.0f));
    }

    // �X�V
    void AttackState::Execute(float elapsedTime)
    {
        // �A�j���[�V�����X�V
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        ownerPos.x += GetMoveSpeed() * elapsedTime;

        // �ǂɂԂ�������Idle�X�e�[�g��
        if (ownerPos.x > 9.0f)
        {
            ownerPos.x = 9.0f;
            owner->GetStateMachine()->SetMoveRight(false);
            owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Recoil));
        }
        if (ownerPos.x < -9.0f)
        {
            ownerPos.x = -9.0f;
            owner->GetStateMachine()->SetMoveRight(true);
            owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Recoil));
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
        SetMoveSpeed(owner->GetStateMachine()->GetMoveRight() ? speed : -speed);
        ownerPos.x += GetMoveSpeed() * elapsedTime;
        recoilCount += GetMoveSpeed() * elapsedTime;

        if (owner->GetStateMachine()->GetMoveRight())
        {
            if (recoilCount > recoil)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Idle));
        }
        else
        {
            if (recoilCount < recoil)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Idle));
        }
        
        owner->GetTransform()->SetPosition(ownerPos);
    }

    // �I��
    void RecoilState::Exit()
    {
        recoilCount = 0;
    }
}

namespace TOFU
{

}

// IdleState
namespace CANNON
{
    // ������
    void IdleState::Enter()
    {
        // materialColor(��)
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

        // �^�C�}�[�Z�b�g
        SetTimer(3.0f);
    }

    // �X�V
    void IdleState::Execute(float elapsedTime)
    {
        // �^�C�}�[��0�ɂȂ�����
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(CANNON::STATE::Attack));

        // �^�C�}�[�����炷
        SubtractTime(elapsedTime);
    }

    // �I��
    void IdleState::Exit()
    {

    }
}

// AttackState
namespace CANNON
{
    // ������
    void AttackState::Enter()
    {
        // materialColor��ݒ�(�U��(��))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    // �X�V   
    void AttackState::Execute(float elapsedTime)
    {
        // todo:�����Œe�ۂ𐶐�����
        //CannonBall* cannonBall = new CannonBall(owner->GetTransform()->GetPosition(), -1, &owner->cannonBallManager);

        // IdleState��
        owner->GetStateMachine()->ChangeState(static_cast<int>(CANNON::STATE::Idle));
    }

    // �I��
    void AttackState::Exit()
    {

    }
}