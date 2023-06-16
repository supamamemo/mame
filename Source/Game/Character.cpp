#include "Character.h"
#include "Collision.h"
#include "Mathf.h"

void Character::DrawDebug()
{
    // ImGui�`��
    model->GetTransform()->DrawDebug();

    ImGui::Begin("debugmodel");
    debugModel->GetTransform()->DrawDebug();
    ImGui::End();
}

// �ړ�����
void Character::Move(
    const float& vx, 
    const float& moveSpeed)
{
    // �ړ������x�N�g����ݒ�
    moveVecX = vx;

    // �ő呬�x�ݒ�
    maxMoveSpeed = moveSpeed; // elapsedTime����Z���Ȃ�
}


// ���񏈗�
void Character::Turn(
    const    float& elapsedTime,
    NO_CONST float vx,  
    NO_CONST float turnSpeed)
{
    // �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (vx == 0.0f) return;

    NO_CONST DirectX::XMFLOAT4 rotation = model->GetTransform()->GetRotation();

    turnSpeed *= elapsedTime;

    // �i�s�x�N�g����P�ʃx�N�g����
    const float vLength = sqrtf(vx * vx);
    if (vLength < 0.001f) return;

    // �P�ʃx�N�g����
    vx /= vLength;

    // ���g�̉�]�l����O���������߂�
    const float frontX = sinf(rotation.y);
    const float frontZ = cosf(rotation.y);

    // ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��όv�Z����
    const float dot = (frontX * vx) /*+ (frontZ * vz)*/;

    // ���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����  
    NO_CONST float rot = 1.0f - dot;   // �␳�l                    
    if (rot > turnSpeed) rot = turnSpeed;

    // ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    const float cross = (frontZ * vx) /*- (frontX * vz)*/;

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    rotation.y += (cross < 0.0f) ? -rot : rot;
    
    model->GetTransform()->SetRotation(rotation);
}


// �W�����v����
void Character::Jump(const float& jumpSpeed)
{
    // ������̗͂�ݒ�
    velocity.y = jumpSpeed;
}


// ���͍X�V����
void Character::UpdateVelocity(const float& elapsedTime)
{
    // �o�߃t���[���i�o�ߎ��Ԃ��ϓ������ꍇ�ł�����Ɍv�Z�����悤�ɂ���j
    const float elapsedFrame = 60.0f * elapsedTime;

    // �������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    // �����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    // �������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);

    // �����ړ��X�V����
    UpdateHorizontalMove(elapsedTime);
}


// ���G���ԍX�V����
void Character::UpdateInvincibleTimer(const float& elapsedTime)
{
    //�@���G���Ԃ��Ȃ���Δ�΂�
    if (invincibleTimer <= 0.0f) return;

    // ���G���Ԃ����炷
    invincibleTimer -= elapsedTime;
}


// �������͍X�V����
void Character::UpdateVerticalVelocity(const float& elapsedFrame)
{
    // �d�͏���
    velocity.y += gravity * elapsedFrame;
}

// �����ړ��X�V����
void Character::UpdateVerticalMove(const float& elapsedTime)
{
    NO_CONST DirectX::XMFLOAT3 position = model->GetTransform()->GetPosition();
    NO_CONST DirectX::XMFLOAT4 rotation = model->GetTransform()->GetRotation();

    // ���������̈ړ���
    const float my = velocity.y * elapsedTime;

    // �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
    NO_CONST DirectX::XMFLOAT3 normal = { 0,1,0 };

    // �X�Η�������
    slopeRate = 0.0f;

    // ������
    if (my < 0.0f)
    {
        // ���C�̊J�n�ʒu�͑�����菭����
        const DirectX::XMFLOAT3 start = {
            position.x,
            position.y + stepOffset,
            position.z
        };
        // ���C�̏I�_�ʒu�͈ړ���̈ʒu
        const DirectX::XMFLOAT3 end = {
            position.x,
            position.y + my,
            position.z
        };

#if 0
        // ���C�L���X�g�ɂ��n�ʔ���
        HitResult hit = {};
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            // �@���x�N�g���擾
            normal = hit.normal;

            // �X�Η��̌v�Z
            const float normalLengthXZ = sqrtf(
                (hit.normal.x * hit.normal.x) +
                (hit.normal.z * hit.normal.z)
            );
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            // �n�ʂɐڒn���Ă���
            //position.y = hit.position.y;
            position = hit.position;

            // ��]
            angle.y += hit.rotation.y;

            // ���n����
            if (!isGround) OnLanding();
            isGround = true;
            velocity.y = 0.0f;
        }
#else 1
        if (end.y <= 0.0f)
        {
            // �o�E���X���͒��˂�����
            if (isBounce)
            {
                OnBounce();
            }
            else
            {
                position.y = 0.0f;

                // ���n����
                if (!isGround) OnLanding();
                isGround = true;
                velocity.y = 0.0f;
            }

        }
#endif
        else
        {
            // �󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
        }
    }
    // �㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    // �n�ʂ̌����ɉ����悤��XZ����]
    {
        // Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
        const float angleX =  atan2f(normal.z, normal.y);
        const float angleZ = -atan2f(normal.x, normal.y);

        // ���`�⊮�Ŋ��炩�ɉ�]����
        rotation.x = Mathf::Lerp(rotation.x, angleX, 0.2f);
        rotation.z = Mathf::Lerp(rotation.z, angleZ, 0.2f);
    }

    model->GetTransform()->SetPosition(position);
    model->GetTransform()->SetRotation(rotation);
}


// �������͍X�V����
void Character::UpdateHorizontalVelocity(const float& elapsedFrame)
{
    const float dist = sqrtf(velocity.x * velocity.x);

    // XZ���ʂ̑��͂���������
    if (dist > 0.0f)
    {
        // ���C��
        float friction = this->friction * elapsedFrame;
        // �󒆂ɂ���Ƃ��͖��C�͂����炷
        if (!isGround) friction *= airControl;

        // ���C�ɂ�鉡�����̌�������
        if (dist > friction)
        {
            // �P�ʃx�N�g����
            const float vx = velocity.x / dist;

            velocity.x -= vx * friction;
        }
        // �������̑��͂����C�͈ȉ��ɂȂ����瑬�͂𖳌���
        else
        {
            velocity.x = 0.0f;
        }
    }


    // X���̑��͂���������
    if (dist <= maxMoveSpeed)
    {
        // �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
        const float moveVecDist = sqrtf(moveVecX * moveVecX);
        if (moveVecDist > 0.0f)
        {
            // ������
            float acceleration = this->acceleration * elapsedFrame;

            // �󒆂ɂ���Ƃ��͉����͂����炷
            if (!isGround) acceleration *= airControl;

            // �ړ��x�N�g���ɂ���������
            velocity.x += moveVecX * acceleration;

            // �ő呬�x����
            const float dist = sqrtf(velocity.x * velocity.x);
            if (dist > maxMoveSpeed)
            {
                velocity.x = moveVecX * maxMoveSpeed;
            }

            // �����ŃK�^�K�^���Ȃ��悤�ɂ���
            if (isGround && slopeRate > 0.0f) // ���n���̓X���[�v�Ή�
            {
                velocity.y -= dist * slopeRate * elapsedFrame;
            }
        }
    }

    // �ړ��x�N�g�������Z�b�g
    moveVecX = 0.0f;
}

// �����ړ��X�V����
void Character::UpdateHorizontalMove(const float& elapsedTime)
{
    NO_CONST DirectX::XMFLOAT3 position = model->GetTransform()->GetPosition();

    // �������͗ʌv�Z
    const float velocityLengthX = sqrtf(velocity.x * velocity.x);

    if (velocityLengthX > 0.0f)
    {
        // �����ړ��l
        const float mx = velocity.x * elapsedTime;

        // ���C�̊J�n�ʒu�ƏI�_�ʒu
        const DirectX::XMFLOAT3 start = {
            position.x,
            position.y + stepOffset,
            position.z
        };
        const DirectX::XMFLOAT3 end = {
            position.x + mx,
            position.y + stepOffset,
            position.z,
        };

        //// ���C�L���X�g�ɂ��ǔ���
        //HitResult hit = {};
        //if (StageManager::Instance().RayCast(start, end, hit))
        //{
        //    // �ǂ܂ł̃x�N�g��
        //    const DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
        //    const DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
        //    const DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
        //
        //    // �ǂ̖@��
        //    const DirectX::XMVECTOR Normal =
        //        DirectX::XMLoadFloat3(&hit.normal);
        //
        //    // ���˃x�N�g����@���Ɏˉe
        //    const DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(
        //        DirectX::XMVectorNegate(Vec), Normal
        //    );
        //
        //    // �␳�ʒu�̌v�Z        
        //    {
        //        //const DirectX::XMVECTOR MovedPosition = DirectX::XMVectorAdd(
        //        //    End, DirectX::XMVectorMultiply(Normal, Dot)
        //        //);          
        //    }
        //    // (v1 * v2) + v3
        //    const DirectX::XMVECTOR MovedPosition =
        //        DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
        //
        //    DirectX::XMFLOAT3 movedPosition = {};
        //    DirectX::XMStoreFloat3(&movedPosition, MovedPosition);
        //
        //    // �ǂ��肵�������ɕǂ��Ȃ�������
        //    HitResult hit2 = {};
        //    if (!StageManager::Instance().RayCast(
        //        hit.position, movedPosition, hit2))
        //    {
        //        // �ǂɓ�����Ȃ���Ε␳�ʒu�Ɉړ�
        //        position.x = movedPosition.x;
        //        position.z = movedPosition.z;
        //    }
        //    else
        //    {
        //        // �ǂɓ��������ʒu�Ɉړ�
        //        position.x = hit2.position.x;
        //        position.z = hit2.position.z;
        //    }
        //}
        //else
        {
            // �ړ�����
            position.x += mx;
        }
    }

    model->GetTransform()->SetPosition(position);
}
