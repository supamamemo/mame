#include "Character.h"
#include "Collision.h"
#include "Mathf.h"
#include "../Mame/Graphics/Graphics.h"

Character::Character()
{
    Graphics& graphics = Graphics::Instance();

    debugModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/cube.fbx", true);
}

void Character::DrawDebug()
{
    // ImGui�`��
    GetTransform()->DrawDebug();

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

    NO_CONST DirectX::XMFLOAT4 rotation = GetTransform()->GetRotation();

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
    
    GetTransform()->SetRotation(rotation);
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


void Character::PlayAnimation(const int& index, const bool& loop, const float& speed)
{
    currentAnimationIndex   = index;    // �Đ�����A�j���[�V�����ԍ���ݒ�
    currentAnimationSeconds = 0.0f;     // �A�j���[�V�����Đ����ԃ��Z�b�g

    animationLoopFlag       = loop;     // ���[�v�����邩
    animationEndFlag        = false;    // �Đ��I���t���O�����Z�b�g

    animationSpeed          = speed;    // �A�j���[�V�����Đ����x
}

void Character::UpdateAnimation(const float& elapsedTime)
{
    // �Đ����łȂ��Ȃ珈�����Ȃ�
    if (!IsPlayAnimation()) return;

    // �ŏI�t���[������(�Đ��I���t���O�������Ă���΍Đ��I��)
    if (animationEndFlag)
    {
        animationEndFlag      = false; // �I���t���O�����Z�b�g
        currentAnimationIndex = -1;    // �A�j���[�V�����ԍ����Z�b�g
        return;
    }

    // �A�j���[�V�����Đ����Ԍo��
    currentAnimationSeconds += elapsedTime;

    // �w��̃A�j���[�V�����f�[�^���擾
    NO_CONST animation& animation = GetAnimation()->at(currentAnimationIndex);

    // ���݂̃t���[�����擾
    const float  frameIndex_float = (currentAnimationSeconds * animation.sampling_rate) * animationSpeed; // �x��������܂Ȃ̂ňꎞ�I��float�ϐ��Ɋi�[
    const size_t frameIndex       = static_cast<const size_t>(frameIndex_float);

    // �Ō�̃t���[�����擾
    const size_t frameEnd = (animation.sequence.size() - 1); 

    // �A�j���[�V�������Đ��������Ă����ꍇ
    if (frameIndex > frameEnd) 
    {
        // ���[�v�t���O�������Ă���΍Đ����Ԃ������߂�
        if (animationLoopFlag)
        {
            currentAnimationSeconds = 0.0f;
            return;
        }
        // ���[�v�Ȃ��Ȃ�Đ��I���t���O�𗧂Ă�
        else
        {
            animationEndFlag = true;
            return;
        }
    }
#if 0 // �������
    else if (!(&keyframe) || frameIndex < frameEnd)
    {
        // �L�[�t���[���擾
        const std::vector<animation::keyframe>& keyframes = animation.sequence;

        const animation::keyframe* keyframeArr[2] = {
            &keyframes.at(frameIndex),
            &keyframes.at(frameIndex + 1)
        };

        // �A�j���[�V���������炩�ɂ�����
        model->skinned_meshes.blend_animations(keyframeArr, 0.01f, keyframe);
    }
#else
    // �A�j���[�V�������Đ��������Ă��Ȃ���Ό��݂̃t���[����ۑ�
    else
    {
        keyframe = animation.sequence.at(frameIndex);
    }
#endif
}


bool Character::IsPlayAnimation() const
{
    if (currentAnimationIndex < 0) return false;

    const int animationIndexEnd = static_cast<int>(model->skinned_meshes.animation_clips.size());
    if (currentAnimationIndex >= animationIndexEnd) return false;

    return true;
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
    NO_CONST DirectX::XMFLOAT3 position = GetTransform()->GetPosition();
    NO_CONST DirectX::XMFLOAT4 rotation = GetTransform()->GetRotation();

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

    GetTransform()->SetPosition(position);
    GetTransform()->SetRotation(rotation);
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

    // �_�b�V�����Ă�����_�b�V���������s��
    if (isDash)
    {
        OnDash();
    }
    // X���̑��͂���������
    else if (dist <= maxMoveSpeed)
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

DirectX::XMFLOAT4X4 Character::SetDebugModelTransform(DirectX::XMFLOAT4X4 transform)
{
    // 0: Target model
    // 1: Bounding box model
    DirectX::XMFLOAT3 dimensions[] = {
#if 1
            {
                model->skinned_meshes.boundingBox[1].x - model->skinned_meshes.boundingBox[0].x,
                model->skinned_meshes.boundingBox[1].y - model->skinned_meshes.boundingBox[0].y,
                model->skinned_meshes.boundingBox[1].z - model->skinned_meshes.boundingBox[0].z,

            },
#else
            { 100.0f, 150.0f, 60.0f },
#endif
            {
                debugModel->skinned_meshes.boundingBox[1].x - debugModel->skinned_meshes.boundingBox[0].x,
                debugModel->skinned_meshes.boundingBox[1].y - debugModel->skinned_meshes.boundingBox[0].y,
                debugModel->skinned_meshes.boundingBox[1].z - debugModel->skinned_meshes.boundingBox[0].z,
            },
    };
    DirectX::XMFLOAT3 centers[] = {
        {
            model->skinned_meshes.boundingBox[0].x + (model->skinned_meshes.boundingBox[1].x - model->skinned_meshes.boundingBox[0].x) * 0.5f,
            model->skinned_meshes.boundingBox[0].y + (model->skinned_meshes.boundingBox[1].y - model->skinned_meshes.boundingBox[0].y) * 0.5f,
            model->skinned_meshes.boundingBox[0].z + (model->skinned_meshes.boundingBox[1].z - model->skinned_meshes.boundingBox[0].z) * 0.5f,
        },
        {
            debugModel->skinned_meshes.boundingBox[0].x + (debugModel->skinned_meshes.boundingBox[1].x - debugModel->skinned_meshes.boundingBox[0].x) * 0.5f,
            debugModel->skinned_meshes.boundingBox[0].y + (debugModel->skinned_meshes.boundingBox[1].y - debugModel->skinned_meshes.boundingBox[0].y) * 0.5f,
            debugModel->skinned_meshes.boundingBox[0].z + (debugModel->skinned_meshes.boundingBox[1].z - debugModel->skinned_meshes.boundingBox[0].z) * 0.5f,
        },
    };

    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(dimensions[0].x / dimensions[1].x, dimensions[0].y / dimensions[1].y, dimensions[0].z / dimensions[1].z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(centers[0].x - centers[1].x, centers[0].y - centers[1].y, centers[0].z - centers[1].z);

    DirectX::XMFLOAT4X4 world; // World transform matrix of bounding box model
    DirectX::XMStoreFloat4x4(&world, S * T * DirectX::XMLoadFloat4x4(&transform/*World transform matrix of target model*/));

    return world;
}
