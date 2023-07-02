#include "Boss.h"
#include "BossStateDerived.h"

#include "PlayerManager.h"

#include "CannonBall.h"
#include "EnemyTofu.h"


// player�����G�͈͓��ɂ��邩���肷��
bool State::FindPlayer()
{
    const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();

    if (player->GetHealth() <= 0) return false; // �v���C���[������ł�����false

    const DirectX::XMFLOAT3& playerPos = player->GetTransform()->GetPosition();
    const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();

    // �v���C���[�Ƃ̋���Y�����G����Y��艓�����false
    const float lengthY = fabsf(playerPos.y - ownerPos.y);
    if (lengthY > owner->GetSerchLengthY()) return false;

    // �v���C���[�Ƃ̋���X�����G����X��艓�����false
    const float lengthX = fabsf(playerPos.x - ownerPos.x);
    if (lengthX > owner->GetSerchLengthX()) return false;

    // �E�������Ă����ԂŃv���C���[���������E�ɂ��邩�A
    // ���������Ă����ԂŃv���C���[��������荶�ɂ����甭�����Ă���̂�true
    if ((owner->GetMoveDirectionX() ==  1.0f && playerPos.x > ownerPos.x) ||
        (owner->GetMoveDirectionX() == -1.0f && playerPos.x < ownerPos.x))
    {
        return true;
    }

    // ������Ȃ�����
    return false;
}


// IdleState
namespace BOSS
{
    // ������
    void IdleState::Enter()
    {               
        // materialColor��ݒ�(�Z�[�t�e�B�[(��))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 0.4f));

        // �ҋ@���Ԃ��Z�b�g
        SetTimer(2.0f);

        // �A�j���[�V�����Z�b�g
        owner->PlayAnimation(static_cast<int>(BossAnimation::Idle), true);
    }

    // �X�V
    void IdleState::Execute(float elapsedTime)
    {
        // �ҋ@���Ԃ�����
        SubtractTime(elapsedTime);

        // �ҋ@���Ԃ��I�������甭���X�e�[�g�֑J��
        if (GetTimer() <= 0.0f)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Find));
            return;
        }

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
        // materialColor(������(���F))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 0.4f));

        // �^�C�}�[���Z�b�g
        SetTimer(1.0f);

        // �A�j���[�V�����Z�b�g
        owner->PlayAnimation(static_cast<int>(BossAnimation::Find), true);
    }

    // �X�V
    void FindState::Execute(float elapsedTime)
    {
        // �^�C�}�[��0�ɂȂ�����AttackState��
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Turn));

        // ���Ԃ����炷
        SubtractTime(elapsedTime);

        owner->CollisionEnemyVsPlayer();    // �v���C���[�Ƃ̏Փ˔��菈��
    }

    // �I��
    void FindState::Exit()
    {
    }
}

// TurnState
namespace BOSS
{
    // ������
    void TurnState::Enter()
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
    void TurnState::Execute(float elapsedTime)
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
    void TurnState::Exit()
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
        // �ړ����x�ݒ�
        owner->SetMoveSpeed(1.0f);        

        // ���񑬓x�ݒ�
        owner->SetTurnSpeed(ToRadian(90.0f));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(TofuAnimation::Walk, true);
    }

    // �X�V
    void WalkState::Execute(float elapsedTime)
    {
        Transform* transform = owner->GetTransform();                

        // �����Ă�������Ɉړ�
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // player�����G�͈͂ɂ����甭���X�e�[�g�֑J��
        if (FindPlayer())
        {
            // �n�ʂ��G�̏�ɏ���Ă�����J�ڂ�����悤�ɂ���(�󒆃W�����v�h�~)
            if (owner->GetIsGround() || owner->GetIsOnFriend())
            {
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
                return;
            }
        }

        // �E�����Ɍ����Ă���ꍇ�A�ړI�n�������ɂ���̂œ��B���Ă��邩���肷��
        if (owner->GetMoveDirectionX() == 1.0f)
        {
            // �ړI�n�𒴂��Ă��������X�e�[�g�֑J��
            if (transform->GetPosition().x >= owner->GetDestination())
            {
                // ���ǐՂ̈ړ��ȂǂŖړI�n��啝�ɒ����Ă����ꍇ�A�ʒu�C�����u�Ԉړ��ɂȂ��Ă��܂��̂ŃR�����g�A�E�g�ɂ��Ă���
                //transform->SetPositionX(owner->GetDestination());       // �ʒu�C��             
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // �ړ������𔽓]
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }
        // �������Ɍ����Ă���ꍇ�A�ړI�n�������ɂ���̂œ��B���Ă��邩���肷��
        else if (owner->GetMoveDirectionX() == -1.0f)
        {
            // �ړI�n�𒴂��Ă��������X�e�[�g�֑J��
            if (transform->GetPosition().x <= owner->GetDestination())
            {
                // ���ǐՂ̈ړ��ȂǂŖړI�n��啝�ɒ����Ă����ꍇ�A�ʒu�C�����u�Ԉړ��ɂȂ��Ă��܂��̂ŃR�����g�A�E�g�ɂ��Ă���
                //transform->SetPositionX(owner->GetDestination());       // �ʒu�C��              
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // �ړ������𔽓]
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }        
    }

    // �I��
    void WalkState::Exit()
    {
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
        const float moveDirectionX = owner->GetMoveDirectionX();
        const float turnSpeed      = owner->GetTurnSpeed();

        // ��]���Ȃ�return
        if (owner->Turn(elapsedTime, moveDirectionX, turnSpeed)) return;
        
        //// ��]���I�������A�͈͓��Ƀv���C���[�������甭���X�e�[�g�֑J��
        //if (State::FindPlayer())
        //{
        //    owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
        //    return;
        //}
        //// ���s�X�e�[�g�֑J��
        //else
        {
            // �ړ��͈͂̒��S����ړ������Ɍ������Ĉړ��͈͂̔��a���i�񂾈ʒu��ړI�n�ɐݒ肷��
            const float moveRangeCenterX = owner->GetMoveRangeCenterX();
            const float moveRangeRadius  = owner->GetMoveRangeRadius();
            owner->SetDestination(moveRangeCenterX + (owner->GetMoveDirectionX() * moveRangeRadius));

            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Walk));
            return;
        }      
    }

    // �I��
    void TurnState::Exit()
    {
    }
}

// FindState
namespace TOFU
{
    // ������
    void FindState::Enter()
    {
        // ���F
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 0.4f));

        owner->SetVelocityX(0.0f); // X���x�����Z�b�g
        owner->Move(0.0f, 0.0f);   // ���ړ����̓��Z�b�g

        // state�����Z�b�g
        state = 0;

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(TofuAnimation::Walk, true);
    }

    // �X�V
    void FindState::Execute(float elapsedTime)
    {
        switch (state)
        {
        case 0: // �W�����v������
            owner->Jump(owner->GetJumpSpeed());
            ++state;
            break;
        case 1: // �n�ʂ��G�̏�ɂ�����ǐՃX�e�[�g�֑J��
            if (owner->GetIsGround() || owner->GetIsOnFriend())
            {
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Track));
                break;
            }
            break;
        }
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
        owner->SetTurnSpeed(ToRadian(540.0f));

        // �ǐՎ��Ԃ�ݒ�
        SetTimer(3.0f);

        owner->PlayAnimation(TofuAnimation::Walk, true);
        owner->SetAnimationSpeed(1.25f); // �A�j���[�V�������x�𑬂߂ɐݒ�
    }

    // �X�V
    void TrackState::Execute(float elapsedTime)
    {
        const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();

        // �v���C���[������ł�����퓬�ҋ@�X�e�[�g�֑J��
        if (player->GetHealth() <= 0)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::IdleBattle));
        }

        // �v���C���[�֌������������Z�o���Ĉړ������ɐݒ�
        {
            const DirectX::XMFLOAT3& playerPos = player->GetTransform()->GetPosition();
            const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();
            const float              vec       = (playerPos.x - ownerPos.x);
            const float              vec_n     = (vec / fabs(vec));
            if (vec_n != owner->GetMoveDirectionX()) owner->SetMoveDirectionX(vec_n);
        }

        // �v���C���[��ǂ�������
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // ���񏈗�
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());

        // �v���C���[�����G�͈͂���O�ꂽ��ǐՎ��Ԃ��������A
        // �ǐՎ��Ԃ��I����������X�e�[�g�֑J�ڂ���(�ʏ펞�ɖ߂�)
        if (!FindPlayer())
        {
            SubtractTime(elapsedTime);
            if (GetTimer() <= 0.0f)
            {
                // �ړ������̐ݒ�
                {
                    // ��������ړ��͈͂̒��S�֌������P�ʃx�N�g�����ړ������ɐݒ�
                    const float moveRangeCenterX = owner->GetMoveRangeCenterX();
                    const float positionX        = owner->GetTransform()->GetPosition().x;
                    const float vec              = (moveRangeCenterX - positionX);
                    const float vec_n            = (vec / fabsf(vec));
                    owner->SetMoveDirectionX(vec_n);
                }

                // ���񑬓x��ʏ�ɖ߂�
                owner->SetTurnSpeed(ToRadian(90.0f));

                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }
        // �v���C���[�����G�͈͂ɂ���ΒǐՎ��Ԃ�ݒ�
        else
        {
            SetTimer(3.0f);
        }
    }

    // �I��
    void TrackState::Exit()
    {
    }
}

// IdleBattleState(�U����ɂ��΂炭�ҋ@����X�e�[�g)
namespace TOFU
{
    void IdleBattleState::Enter()
    {
        // �ړ����x��ݒ�(��~)
        owner->SetMoveSpeed(0.0f);

        // �퓬�ҋ@���Ԃ�ݒ�
        SetTimer(3.0f);

        // �Đ���~
        owner->PlayAnimation(TofuAnimation::Turn, true, 1.25f);
    }

    void IdleBattleState::Execute(float elapsedTime)
    {
        // ���񏈗�
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());
        
        SubtractTime(elapsedTime); // �ҋ@���Ԍ���

        if (GetTimer() > 0.0f) return;

        // �v���C���[���͈͓��ɂ����甭���X�e�[�g�֑J��
        if (FindPlayer())
        {
            // �n�ʂ��G�̏�ɏ���Ă�����J�ڂ�����悤�ɂ���(�󒆃W�����v�h�~)
            if (owner->GetIsGround() || owner->GetIsOnFriend())
            {
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
                return;
            }
        }
        // �v���C���[���͈͓��ɂ��Ȃ��đҋ@���Ԃ��I�����Ă���ΐ���X�e�[�g�֑J��(�ʏ펞�ɖ߂�)
        else 
        {
            // �ړ������̐ݒ�
            {
                // ��������ړ��͈͂̒��S�֌������P�ʃx�N�g�����ړ������ɐݒ�
                const float moveRangeCenterX = owner->GetMoveRangeCenterX();
                const float positionX        = owner->GetTransform()->GetPosition().x;
                const float vec              = (moveRangeCenterX - positionX);
                const float vec_n            = (vec / fabsf(vec));
                owner->SetMoveDirectionX(vec_n);
            }

            // ���񑬓x��ʏ�ɖ߂�
            owner->SetTurnSpeed(ToRadian(90.0f));

            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
            return;
        }
    }

    void IdleBattleState::Exit()
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