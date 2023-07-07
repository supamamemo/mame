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
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
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
        owner->PlayAnimation(static_cast<int>(BossAnimation::AttackBefore), true);
    }

    // �X�V
    void FindState::Execute(float elapsedTime)
    {
        // ���Ԃ����炷
        SubtractTime(elapsedTime);

        // �^�C�}�[��0�ɂȂ��������X�e�[�g�֑J��
        if (GetTimer() < 0.0f)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
            return;
        }

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
        //rotate = playerPos.x > ownerPos.x ? 90.0f : 270.0f;
    
        // �i�ޕ�����ݒ肷��
        //owner->SetMoveRight(playerPos.x > ownerPos.x ? true : false);

        // �v���C���[���ǂ����ɂ���̂�
        const DirectX::XMFLOAT3& playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();
        const float vecX    = (playerPos.x - ownerPos.x);
        const float vecX_n  = (vecX / fabsf(vecX));

        // �i�ޕ�����ݒ肷��
        owner->SetMoveDirectionX(vecX_n);
    }

    // �X�V
    void TurnState::Execute(float elapsedTime)
    {
        // �v���C���[�̕����Ɍ����悤�ɉ�]����
        {
            //DirectX::XMFLOAT4 rotation = owner->GetTransform()->GetRotation();
            //// 90�x�̏ꍇ
            //if (rotate == 90.0f)
            //{
            //    // ��]
            //    //rotation.y -= GetRotationSpeed() * elapsedTime;
            //    
            //    // ��]�I�������AttackState��
            //    if (DirectX::XMConvertToRadians(rotate) >= rotation.y)
            //    {
            //        rotation.y = DirectX::XMConvertToRadians(90.0f);
            //        owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Attack));
            //    }
            //}
            //// -90�x�̏ꍇ
            //else
            //{
            //    // ��]
            //    //rotation.y += GetRotationSpeed() * elapsedTime;

            //    // ��]�I�������AttackState��
            //    if (DirectX::XMConvertToRadians(rotate) <= rotation.y)
            //    {
            //        rotation.y = DirectX::XMConvertToRadians(270.0f);
            //        owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
            //    }
            //}
            //owner->GetTransform()->SetRotation(rotation);
        }
        // ��]���I�������U���X�e�[�g�֑J��
        const float moveDirectionX = owner->GetMoveDirectionX();
        const float turnSpeed      = owner->GetTurnSpeed();      
        if (!owner->Turn(elapsedTime, moveDirectionX, turnSpeed)) 
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
            return;
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
        // materialColor��ݒ�(�A�O���b�V�u(��))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 0.4f));

        // ���E����
        //owner->SetMoveSpeed(owner->GetMoveRight() ? speed : -speed);
        
        // �A�j���[�V�����Z�b�g
        owner->PlayAnimation(static_cast<int>(BossAnimation::Attack), true);
    }

    // �X�V
    void AttackState::Execute(float elapsedTime)
    {
        //const DirectX::XMFLOAT3& ownerPos = owner->GetTransform()->GetPosition();
        Transform* transform = owner->GetTransform();
        
        // �ړ�
        //ownerPos.x += owner->GetMoveSpeed() * elapsedTime;
        owner->SetMoveSpeed(owner->GetMoveDirectionX() * speed);
        transform->AddPositionX(owner->GetMoveSpeed() * elapsedTime);

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
        // materialColor��ݒ�(��)
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 0.4f));
        
        // ���݂̔������������Z�b�g
        currentRecoilLength = 0.0f;
        
        //SetRecoil(owner->GetStateMachine()->GetMoveRight() ? 1.0f : -1.0f);
        //SetRecoil(owner->GetMoveDirectionX());

        // ���G��Ԃ𖳂���
        owner->SetIsInvincible(false);

        // �A�j���[�V�����Z�b�g
        owner->PlayAnimation(static_cast<int>(BossAnimation::Recoil), true);
    }

    // �X�V
    void RecoilState::Execute(float elapsedTime)
    {
        Transform* transform = owner->GetTransform();

        //owner->SetMoveSpeed(owner->GetStateMachine()->GetMoveRight() ? speed : -speed);

        //if (owner->GetStateMachine()->GetMoveRight())
        if (currentRecoilLength < recoilLength)
        {
            owner->SetMoveSpeed(owner->GetMoveDirectionX() * speed);

            transform->AddPositionX(owner->GetMoveSpeed() * elapsedTime);
            currentRecoilLength += fabsf(owner->GetMoveSpeed() * elapsedTime);

            if (currentRecoilLength >= recoilLength) SetTimer(1.0f);
        }
        else
        {
            SubtractTime(elapsedTime);
            if (GetTimer() <= 0.0f) owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));
        }
        
        //owner->GetTransform()->SetPosition(ownerPos);

        // �����蔻��
        if(Collision::IntersectAABBVsAABB(PlayerManager::Instance().GetPlayer()->aabb_, owner->aabb_))
        { 
           // todo : �����蔻�� 
        }
    }

    // �I��
    void RecoilState::Exit()
    {
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

        // �n�`�̒[�𒴂������ɂȂ��������X�e�[�g�֑J��
        {
            if (owner->lastLandingTerrainAABB_.max.x != 0.0f && 
                owner->aabb_.max.x > owner->lastLandingTerrainAABB_.max.x)
            {
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // �ړ������𔽓]
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
            else if (owner->lastLandingTerrainAABB_.min.x != 0.0f &&
                owner->aabb_.min.x < owner->lastLandingTerrainAABB_.min.x)
            {
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // �ړ������𔽓]
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
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
            owner->SetIsOnFriend(false); // �����̏�ɏ�������Ă��邩�t���O�����Z�b�g����
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

        // �v���C���[�֌������������Z�o���Ĉړ������ɐݒ�
        {
            const DirectX::XMFLOAT3& playerPos = player->GetTransform()->GetPosition();
            const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();
            const float vecX      = (playerPos.x - ownerPos.x);
            const float lengthX   = fabsf(vecX);
            // �������[���ȉ����Ɠ��������ł���̂Ŗh�~
            if (lengthX > 0.0f)
            {
                const float vecX_n = (vecX / lengthX);
                owner->SetMoveDirectionX(vecX_n);
            }
        }

        // �v���C���[��ǂ�������
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // ���񏈗�
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());

        // �v���C���[�����G�͈͂���O�ꂽ��ǐՎ��Ԃ��������A
        // �ǐՎ��Ԃ��I����������X�e�[�g�֑J�ڂ���(�ʏ펞�ɖ߂�)
        if (player->GetHealth() <= 0 || !FindPlayer())
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

// DeathState(���S�X�e�[�g)
namespace TOFU
{
    void DeathState::Enter()
    {
        owner->SetMoveSpeed(10.0f);             // XZ�����̐�����ї͂�ݒ�

        owner->SetJumpSpeed(20.0f);             // Y�����̐�����ї͐ݒ�
        owner->Jump(owner->GetJumpSpeed());     // ��ɐ�����΂�

        owner->SetTurnSpeed(ToRadian(540.0f)); // ��]���x�ݒ�
    }

    void DeathState::Execute(float elapsedTime)
    {
        Transform* transform = owner->GetTransform();

        // ������΂�
        const float moveDirectionX  = owner->GetMoveDirectionX();           // ������ԕ���X(OnDead�֐��Őݒ�ς�)
        const float impulse         = owner->GetMoveSpeed() * elapsedTime;  // ������ї�
        const float velocityX       = moveDirectionX * impulse;             // ���ɐ�����΂�
        const float velocityZ       = -impulse;                             // ��O�ɐ�����΂�
        transform->AddPosition(DirectX::XMFLOAT3(velocityX, 0.0f, velocityZ));

        // ��]������
        const float rotate = moveDirectionX * owner->GetTurnSpeed() * elapsedTime;
        transform->AddRotationX(rotate);
        transform->AddRotationZ(rotate);

        // (���ŏ�����OnFallDead�֐��ōs���Ă���)
    }

    void DeathState::Exit()
    {
    }
}



// WalkState
namespace RED_TOFU
{
    // ������
    void WalkState::Enter()
    {
        // �ړ����x�ݒ�
        owner->SetMoveSpeed(2.0f);

        // �����͐ݒ�
        owner->SetAcceleration(1.0f);

        // ���C�͐ݒ�
        owner->SetFriction(0.5f);

        // ���񑬓x�ݒ�
        owner->SetTurnSpeed(ToRadian(180.0f));

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

        // �n�`�̒[�𒴂������ɂȂ��������X�e�[�g�֑J��
        {
            if (owner->lastLandingTerrainAABB_.max.x != 0.0f &&
                owner->aabb_.max.x > owner->lastLandingTerrainAABB_.max.x)
            {
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // �ړ������𔽓]
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
            else if (owner->lastLandingTerrainAABB_.min.x != 0.0f &&
                owner->aabb_.min.x < owner->lastLandingTerrainAABB_.min.x)
            {
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // �ړ������𔽓]
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
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
namespace RED_TOFU
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


        // �n�`�̒[�𒴂��ė��������ɂȂ�����C��
        {
            if (owner->lastLandingTerrainAABB_.max.x != 0.0f && owner->aabb_.max.x > owner->lastLandingTerrainAABB_.max.x)
            {
                const float fixLeft = -fabsf(owner->aabb_.max.x - owner->lastLandingTerrainAABB_.max.x);
                owner->GetTransform()->AddPositionX(fixLeft);
            }
            else if (owner->lastLandingTerrainAABB_.min.x != 0.0f && owner->aabb_.min.x < owner->lastLandingTerrainAABB_.min.x)
            {
                const float fixRight = fabsf(owner->aabb_.min.x - owner->lastLandingTerrainAABB_.min.x);
                owner->GetTransform()->AddPositionX(fixRight);
            }
        }

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
namespace RED_TOFU
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
            owner->SetIsOnFriend(false); // �����̏�ɏ�������Ă��邩�t���O�����Z�b�g����
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
namespace RED_TOFU
{
    // ������
    void TrackState::Enter()
    {
        // �ԐF
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.4f));

        // ���x��ݒ�
        owner->SetMoveSpeed(4.0f);

        // �����͐ݒ�(��߂ɐݒ�)
        owner->SetAcceleration(0.7f);        
        
        // ���C�͐ݒ�(��߂ɐݒ�)
        owner->SetFriction(0.1f);

        // ��]���x��ݒ�
        owner->SetTurnSpeed(ToRadian(540.0f));

        //// �ǐՎ��Ԃ�ݒ�
        //SetTimer(3.0f);

        owner->PlayAnimation(TofuAnimation::Walk, true);
        owner->SetAnimationSpeed(1.5f); // �A�j���[�V�������x�𑬂߂ɐݒ�
    }

    // �X�V
    void TrackState::Execute(float elapsedTime)
    {
        const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();

        if (player->GetHealth() <= 0)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::IdleBattle));
        }

        // �v���C���[�֌������������Z�o���Ĉړ������ɐݒ�
        {
            const DirectX::XMFLOAT3& playerPos = player->GetTransform()->GetPosition();
            const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();
            const float vecX      = (playerPos.x - ownerPos.x);
            const float lengthX   = fabsf(vecX);
            // �������[���ȉ����Ɠ��������ł���̂Ŗh�~
            if (lengthX > 0.0f)
            {
                const float vecX_n = (vecX / lengthX);
                owner->SetMoveDirectionX(vecX_n);
            }
        }

        // �v���C���[��ǂ�������
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // ���񏈗�
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());

    }

    // �I��
    void TrackState::Exit()
    {
    }
}

// IdleBattleState(�U����ɂ��΂炭�ҋ@����X�e�[�g)
namespace RED_TOFU
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

        // �v���C���[�������Ă����甭���X�e�[�g�֑J��
        const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();
        if (player->GetHealth() > 0)
        {
            // �n�ʂ��G�̏�ɏ���Ă�����J�ڂ�����悤�ɂ���(�󒆃W�����v�h�~)
            if (owner->GetIsGround() || owner->GetIsOnFriend())
            {
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
                return;
            }
        }
        // �v���C���[������ł��������X�e�[�g�֑J��(�ʏ펞�ɖ߂�)
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

// DeathState(���S�X�e�[�g)
namespace RED_TOFU
{
    void DeathState::Enter()
    {
        owner->SetMoveSpeed(10.0f);             // XZ�����̐�����ї͂�ݒ�

        owner->SetJumpSpeed(20.0f);             // Y�����̐�����ї͐ݒ�
        owner->Jump(owner->GetJumpSpeed());     // ��ɐ�����΂�

        owner->SetTurnSpeed(ToRadian(540.0f)); // ��]���x�ݒ�
    }

    void DeathState::Execute(float elapsedTime)
    {
        Transform* transform = owner->GetTransform();

        // ������΂�
        const float moveDirectionX  = owner->GetMoveDirectionX();           // ������ԕ���X(OnDead�֐��Őݒ�ς�)
        const float impulse         = owner->GetMoveSpeed() * elapsedTime;  // ������ї�
        const float velocityX       = moveDirectionX * impulse;             // ���ɐ�����΂�
        const float velocityZ       = -impulse;                             // ��O�ɐ�����΂�
        transform->AddPosition(DirectX::XMFLOAT3(velocityX, 0.0f, velocityZ));

        // ��]������
        const float rotate = moveDirectionX * owner->GetTurnSpeed() * elapsedTime;
        transform->AddRotationX(rotate);
        transform->AddRotationZ(rotate);

        // (���ŏ�����OnFallDead�֐��ōs���Ă���)
    }

    void DeathState::Exit()
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