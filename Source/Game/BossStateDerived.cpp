#include "Boss.h"
#include "BossStateDerived.h"

#include "PlayerManager.h"

#include "CannonBall.h"
#include "EnemyTofu.h"

// IdleState
namespace BOSS
{
    // ������
    void IdleState::Enter()
    {
        // �A�j���[�V�����Z�b�g
        owner->PlayAnimation(static_cast<int>(BossAnimation::Idle), true);
                
        // materialColor��ݒ�(�Z�[�t�e�B�[(��))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 0.4f));

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

        owner->CollisionEnemyVsPlayer();    // �v���C���[�Ƃ̏Փ˔��菈��
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 0.4f));

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

        owner->CollisionEnemyVsPlayer();    // �v���C���[�Ƃ̏Փ˔��菈��
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.4f));

        // ��]���x�ݒ�
        //SetRotationSpeed(3.0f);

        // �v���C���[���ǂ����ɂ���̂�
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        rotate = playerPos.x > ownerPos.x ? 90.0f : 270.0f;
        
        // �i�ޕ�����ݒ肷��
        owner->SetMoveRight(playerPos.x > ownerPos.x ? true : false);
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
                //rotation.y -= GetRotationSpeed() * elapsedTime;
                
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
                //rotation.y += GetRotationSpeed() * elapsedTime;

                // ��]�I�������AttackState��
                if (DirectX::XMConvertToRadians(rotate) <= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(270.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
                }
            }
            owner->GetTransform()->SetRotation(rotation);
        }

        owner->CollisionEnemyVsPlayer();    // �v���C���[�Ƃ̏Փ˔��菈��
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 0.4f));

        // ���E����
        owner->SetMoveSpeed(owner->GetMoveRight() ? speed : -speed);
    }

    // �X�V
    void AttackState::Execute(float elapsedTime)
    {
        // �A�j���[�V�����X�V
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        ownerPos.x += owner->GetMoveSpeed() * elapsedTime;

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

        owner->CollisionEnemyVsPlayer();    // �v���C���[�Ƃ̏Փ˔��菈��
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 0.4f));
        
        // ����������ݒ�
        recoilCount = 0;
        
        SetRecoil(owner->GetStateMachine()->GetMoveRight() ? 1.0f : -1.0f);

        // ���G��Ԃ𖳂���
        owner->SetIsInvincible(false);
    }

    // �X�V
    void RecoilState::Execute(float elapsedTime)
    {
        // �A�j���[�V�����X�V
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        owner->SetMoveSpeed(owner->GetStateMachine()->GetMoveRight() ? speed : -speed);
        ownerPos.x += owner->GetMoveSpeed() * elapsedTime;
        recoilCount += owner->GetMoveSpeed() * elapsedTime;

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

        // ���G��Ԃɂ���
        owner->SetIsInvincible(true);
    }
}


// WalkState
namespace TOFU
{
    // ������
    void WalkState::Enter()
    {
        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(TofuAnimation::Walk, true);

        // �ړ����x�ݒ�
        owner->SetMoveSpeed(5.0f);        

        // �ړI�n��ݒ�
/*        destination = owner->GetMoveRight()
                    ? owner->GetTransform()->GetPosition().x + owner->GetMoveRangeRadius()
                    : owner->GetTransform()->GetPosition().x - 6.0f;    */    

        // �ړ������E�ړ��͈͂̔��a�E�ړ��͈͂̒��S���擾
        const float moveDirectionX  = owner->GetMoveDirectionX();
        const float moveRangeCenter = owner->GetMoveRangeCenterX();
        const float moveRangeRadius = owner->GetMoveRangeRadius();

        // �ړI�n��ݒ�
        destination = moveDirectionX * (moveRangeCenter + moveRangeRadius);
    }

    // �X�V
    void WalkState::Execute(float elapsedTime)
    {
        // �ړ�����
        {
            Transform*   transform      = owner->GetTransform();                    // �g�����X�t�H�[���擾
            const float  moveDirectionX = owner->GetMoveDirectionX();               // �ړ��������擾
            const float  velocityX      = (owner->GetMoveSpeed() * elapsedTime);    // ���x���v�Z

            // �����Ă�������Ɉړ�
            transform->AddPositionX(moveDirectionX * velocityX);

            // �E�����Ɍ����Ă���ꍇ�A�ړI�n�������ɂ���̂œ��B���Ă��邩���肷��
            if (moveDirectionX == 1.0f)
            {
                // �ړI�n�𒴂��Ă��������X�e�[�g�֑J��
                if (transform->GetPosition().x > destination)
                {
                    transform->SetPositionX(destination);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Turn));
                }
            }
            // �������Ɍ����Ă���ꍇ�A�ړI�n�������ɂ���̂œ��B���Ă��邩���肷��
            else if (moveDirectionX == -1.0f)
            {
                // �ړI�n�𒴂��Ă��������X�e�[�g�֑J��
                if (transform->GetPosition().x < destination)
                {
                    transform->SetPositionX(destination);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Turn));
                }
            }
            
            //if (owner->GetMoveRight())
            //{
            //    transform->AddPositionX(velocityX);
            //    if (transform->GetPosition().x >= destination)
            //    {
            //        owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Turn));
            //    }
            //}
            //else
            //{
            //    transform->AddPositionX(-velocityX);
            //    if (transform->GetPosition().x <= destination)
            //    {
            //        owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Turn));
            //    }
            //}
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
        const DirectX::XMFLOAT3& playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();

        // �v���C���[�Ƃ̋������Z�o
        const float vec    = playerPos.x - ownerPos.x;
        const float length = sqrtf(vec * vec);

        // �v���C���[�Ƃ̋��������G������艓�����return
        if (length > owner->GetSerchLength()) return;

        ////�@�E�������Ă����ԂŃv���C���[���������E�ɂ����甭���X�e�[�g�֑J��
        //if (owner->GetMoveRight() && playerPos.x > ownerPos.x)
        //{
        //    owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Find));
        //}
        ////�@���������Ă����ԂŃv���C���[��������荶�ɂ����甭���X�e�[�g�֑J��
        //else if (!owner->GetMoveRight() && playerPos.x < ownerPos.x)
        //{
        //    owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Find));
        //}
    }
}

// TurnState
namespace TOFU
{
    // ������
    void TurnState::Enter()
    {
        // ����A�j���[�V�����ݒ�
        owner->PlayAnimation(TofuAnimation::Turn, true);
    }

    // �X�V
    void TurnState::Execute(float elapsedTime)
    {
        // ���ɐi�ޕ����։�]����
        {
            owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());

            //DirectX::XMFLOAT4 rotation = owner->GetTransform()->GetRotation();
            //// 90�x�̏ꍇ
            //if (rotate == 90.0f)
            //{
            //    // ��]
            //    rotation.y -= GetRotationSpeed() * elapsedTime;

            //    // ��]�I�������AttackState��
            //    if (DirectX::XMConvertToRadians(rotate) >= rotation.y)
            //    {
            //        rotation.y = DirectX::XMConvertToRadians(90.0f);
            //        owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Walk));
            //    }
            //}
            //// 270�x�̏ꍇ
            //else
            //{
            //    // ��]
            //    rotation.y += GetRotationSpeed() * elapsedTime;

            //    // ��]�I�������AttackState��
            //    if (DirectX::XMConvertToRadians(rotate) <= rotation.y)
            //    {
            //        rotation.y = DirectX::XMConvertToRadians(270.0f);
            //        owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Walk));
            //    }
            //}
            //owner->GetTransform()->SetRotation(rotation);
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
        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(TofuAnimation::Walk, true);

        // ���F
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 0.4f));

        // ������n�_Y��ۑ�����
        returnPositionY = owner->GetTransform()->GetPosition().y;

        // state�����Z�b�g
        state = 0;

        // ��ђ��˂鑬�x��ݒ肷��
        owner->SetMoveSpeed(10.0f);
    }

    // �X�V
    void FindState::Execute(float elapsedTime)
    {
        DirectX::XMFLOAT3 pos = owner->GetTransform()->GetPosition();
        switch (state)
        {
        case 0:
            pos.y += owner->GetMoveSpeed() * elapsedTime;
            if (pos.y > returnPositionY + 2.0f)state = 1;
            break;
        case 1:
            pos.y -= owner->GetMoveSpeed() * elapsedTime;
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.4f));

        // ���x��ݒ�
        owner->SetMoveSpeed(2.0f);

        // ��]���x��ݒ�
        //SetRotationSpeed(10.0f);
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
            ownerPos.x += owner->GetMoveSpeed() * elapsedTime;
            
            // �E�ɐi�ނ̂ŉ�]Y��90�x�ɂȂ�悤�ɐݒ肷��
            if (ownerRot.y != DirectX::XMConvertToRadians(90))
            {
                //ownerRot.y -= GetRotationSpeed() * elapsedTime;
                
                // 90�x��菬�����Ȃ�����90�x�ɐݒ肷��
                if (ownerRot.y <= DirectX::XMConvertToRadians(90))
                    ownerRot.y = DirectX::XMConvertToRadians(90);
            }
        }
        else
        {
            ownerPos.x -= owner->GetMoveSpeed() * elapsedTime;

            // ���ɐi�ނ̂ŉ�]Y��270�x�ɂȂ�悤�ɐݒ肷��
            if (ownerRot.y != DirectX::XMConvertToRadians(270))
            {
                //ownerRot.y += GetRotationSpeed() * elapsedTime;

                // 270�x���傫���Ȃ�����270�x�ɐݒ肷��
                if (ownerRot.y >= DirectX::XMConvertToRadians(270))
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.4f));

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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.4f));
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