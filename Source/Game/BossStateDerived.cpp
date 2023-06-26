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
    }

    // �X�V
    void FindState::Execute(float elapsedTime)
    {
        // �A�j���[�V�����X�V
        owner->UpdateAnimation(elapsedTime);

        // �^�C�}�[��0�ɂȂ�����AttackState��
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Rotate));

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
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

        // ��]���x�ݒ�
        SetRotationSpeed(3.0f);

        // �v���C���[���ǂ����ɂ���̂�
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        rotate = playerPos.x > ownerPos.x ? 90.0f : 270.0f;
    }

    // �X�V
    void RotateState::Execute(float elapsedTime)
    {
        // �v���C���[�̕����Ɍ����悤�ɉ�]����
        {
            DirectX::XMFLOAT4 rotation = owner->GetTransform()->GetRotation();
            // 90�x�̏ꍇ
            if (rotate == 90.0f)
            {
                // ��]
                rotation.y -= GetRotationSpeed() * elapsedTime;
                
                // ��]�I�������AttackState��
                if (DirectX::XMConvertToRadians(rotate) >= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(90.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Attack));
                }
            }
            // -90�x�̏ꍇ
            else
            {
                // ��]
                rotation.y += GetRotationSpeed() * elapsedTime;

                // ��]�I�������AttackState��
                if (DirectX::XMConvertToRadians(rotate) <= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(270.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
                }
            }
            owner->GetTransform()->SetRotation(rotation);
        }
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

// WalkState
namespace TOFU
{
    // ������
    void WalkState::Enter()
    {
        // �ړ����x�ݒ�
        SetMoveSpeed(1.0f);        

        // �ړI�n��ݒ�
        destination = owner->GetMoveRight() ? owner->GetTransform()->GetPosition().x + 10.0f
            : owner->GetTransform()->GetPosition().x - 10.0f;
    }

    // �X�V
    void WalkState::Execute(float elapsedTime)
    {
        // �ړ�����
        {
            DirectX::XMFLOAT3 pos = owner->GetTransform()->GetPosition();
            if (owner->GetMoveRight())
            {
                pos.x += GetMoveSpeed() * elapsedTime;
                if (pos.x >= destination)owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Turn));
            }
            else
            {
                pos.x -= GetMoveSpeed() * elapsedTime;
                if (pos.x <= destination)owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Turn));
            }
            owner->GetTransform()->SetPosition(pos);
        }

        // player�Ƃ͈̔̓`�F�b�N
        FindPlayer();
    }

    // �I��
    void WalkState::Exit()
    {
    }

    // player�Ƃ̋������v�Z���ċ߂��ɂ����FindState��
    void WalkState::FindPlayer()
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        // owner����player�ւ̃x�N�g�����Z�o
        DirectX::XMVECTOR VEC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&ownerPos));
        DirectX::XMFLOAT3 vec{};
        DirectX::XMStoreFloat3(&vec, VEC);

        // ���������͈͊O�Ȃ̂Ŕ�����s��Ȃ�
        float vecX = vec.x;
        vecX = (vecX > 0) ? vecX : -vecX;
        if (vecX > 4.0f)return;

        // owner:�i�s�����E,player:�E�͈͓̔��ɂ���
        if (owner->GetMoveRight() && vec.x > 0)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Find));
        }
        // owner:�i�s������,player:���͈͓̔��ɂ���
        if (!owner->GetMoveRight() && vec.x < 0)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Find));
        }
    }
}

// TurnState
namespace TOFU
{
    // ������
    void TurnState::Enter()
    {
        // ��]���x�ݒ�
        SetRotationSpeed(3.0f);

        // ��]�����ݒ�
        rotate = owner->GetMoveRight() ? 270.0f : 90.0f;
    }

    // �X�V
    void TurnState::Execute(float elapsedTime)
    {
        // ���ɐi�ޕ����։�]����
        {
            DirectX::XMFLOAT4 rotation = owner->GetTransform()->GetRotation();
            // 90�x�̏ꍇ
            if (rotate == 90.0f)
            {
                // ��]
                rotation.y -= GetRotationSpeed() * elapsedTime;

                // ��]�I�������AttackState��
                if (DirectX::XMConvertToRadians(rotate) >= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(90.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Walk));
                }
            }
            // 270�x�̏ꍇ
            else
            {
                // ��]
                rotation.y += GetRotationSpeed() * elapsedTime;

                // ��]�I�������AttackState��
                if (DirectX::XMConvertToRadians(rotate) <= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(270.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Walk));
                }
            }
            owner->GetTransform()->SetRotation(rotation);
        }
    }

    // �I��
    void TurnState::Exit()
    {
        owner->SetMoveRight(owner->GetMoveRight() ? false : true);
    }
}

// FindState
namespace TOFU
{
    // ������
    void FindState::Enter()
    {
        // ���F
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));

        // ������n�_Y��ۑ�����
        returnPositionY = owner->GetTransform()->GetPosition().y;

        // state�����Z�b�g
        state = 0;

        // ��ђ��˂鑬�x��ݒ肷��
        SetMoveSpeed(10.0f);
    }

    // �X�V
    void FindState::Execute(float elapsedTime)
    {
        DirectX::XMFLOAT3 pos = owner->GetTransform()->GetPosition();
        switch (state)
        {
        case 0:
            pos.y += GetMoveSpeed() * elapsedTime;
            if (pos.y > returnPositionY + 2.0f)state = 1;
            break;
        case 1:
            pos.y -= GetMoveSpeed() * elapsedTime;
            if (pos.y < returnPositionY)
                owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Track));
            break;
        }
        owner->GetTransform()->SetPosition(pos);
    }

    // �I��
    void FindState::Exit()
    {

    }
}

// TrackState
namespace TOFU
{
    // ������
    void TrackState::Enter()
    {
        // �ԐF
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

        // ���x��ݒ�
        SetMoveSpeed(2.0f);

        // ��]���x��ݒ�
        SetRotationSpeed(10.0f);
    }

    // �X�V
    void TrackState::Execute(float elapsedTime)
    {
        // �ǐՂ��邽�߂Ɉʒu���Ƃ�
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        DirectX::XMVECTOR VEC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&ownerPos));
        DirectX::XMFLOAT3 vec{};
        DirectX::XMStoreFloat3(&vec, VEC);

        // �ǐՂ̕����ɉ���߂̉�]
        DirectX::XMFLOAT4 ownerRot = owner->GetTransform()->GetRotation();

        if (vec.x > 0)
        {
            ownerPos.x += GetMoveSpeed() * elapsedTime;
            
            // �E�ɐi�ނ̂ŉ�]Y��90�x�ɂȂ�悤�ɐݒ肷��
            if (ownerRot.y != DirectX::XMConvertToRadians(90))
            {
                ownerRot.y -= GetRotationSpeed() * elapsedTime;
                
                // 90�x��菬�����Ȃ�����90�x�ɐݒ肷��
                if (ownerRot.y < DirectX::XMConvertToRadians(90))
                    ownerRot.y = DirectX::XMConvertToRadians(90);
            }
        }
        else
        {
            ownerPos.x -= GetMoveSpeed() * elapsedTime;

            // ���ɐi�ނ̂ŉ�]Y��270�x�ɂȂ�悤�ɐݒ肷��
            if (ownerRot.y != DirectX::XMConvertToRadians(270))
            {
                ownerRot.y += GetRotationSpeed() * elapsedTime;

                // 270�x���傫���Ȃ�����270�x�ɐݒ肷��
                if (ownerRot.y > DirectX::XMConvertToRadians(270))
                    ownerRot.y = DirectX::XMConvertToRadians(270);
            }
        }

        owner->GetTransform()->SetPosition(ownerPos);
        owner->GetTransform()->SetRotation(ownerRot);
    }

    // �I��
    void TrackState::Exit()
    {
    }
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