#include "TitleCharacter.h"
#include "../Mame/Graphics/Graphics.h"
#include <DirectXMath.h>

void TitleCharacter::Render(const float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // world�s��X�V
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model_->GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��
    if (&model_->keyframe)
    {
        model_->skinned_meshes->render(
            graphics.GetDeviceContext(), 
            transform, 
            DirectX::XMFLOAT4(1,1,1,1), 
            &model_->keyframe
        );
    }
    else
    {
        model_->skinned_meshes->render(
            graphics.GetDeviceContext(), 
            transform, 
            DirectX::XMFLOAT4(1,1,1,1), 
            nullptr
        );
    }
}

void TitleCharacter::DrawDebug()
{
    GetTransform()->DrawDebug();    // ImGui�`��
}


void TitleCharacter::CircularMotion(const float elapsedTime)
{
    Transform* transform = GetTransform();

    // ��]������
    rotationY_ += rotationSpeedY_ * elapsedTime;

    // �P�ʉ~�i�P�ʃx�N�g���j�����߂�
    const float unitCircleX = sinf(rotationY_);  // sinf�F���S����X���Ɍ������P�ʉ~�����߂�
    const float unitCircleZ = cosf(rotationY_);  // cosf�F���S����Y��(Z��)�Ɍ������P�ʉ~�����߂�

    // �~�̒��S����P�ʉ~�̕����Ɍ������ĉ~�̔��a���i�񂾈ʒu�����߂�
    const float posX = circle_.center.x + unitCircleX * circle_.radius;
    const float posZ = circle_.center.z + unitCircleZ * circle_.radius;

    // �ʒu���X�V
    transform->SetPositionX(posX);
    transform->SetPositionZ(posZ);

    // �v���C���[��i�s�����Ɍ�������
    const float moveVecX = sinf(rotationY_ + ToRadian(-90.0f)); // �i�s�����Ɍ������邽�߂ɒǉ���-90�x�Ȃ��Ă���
    const float moveVecZ = cosf(rotationY_ + ToRadian(-90.0f)); // �i�s�����Ɍ������邽�߂ɒǉ���-90�x�Ȃ��Ă���
    transform->SetRotationY(atan2f(moveVecX, moveVecZ));        // atan2f�F���W����p�x�����߂�

    //Turn(elapsedTime, turnVecX, turnVecZ, ToRadian(360.0f));
}


void TitleCharacter::Turn(
    const float elapsedTime,
    NO_CONST float vx,
    NO_CONST float vz,
    NO_CONST float turnSpeed)
{
    // �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (vx == 0.0f && vz == 0.0f) return;

    turnSpeed *= elapsedTime;

    // �i�s�x�N�g����P�ʃx�N�g����
    const float vLength = sqrtf((vx * vx) + (vz * vz));
    if (vLength < 0.001f) return;

    // �P�ʃx�N�g����
    vx /= vLength;
    vz /= vLength;

    // ���g�̉�]�l����O���������߂�
    const float frontX = sinf(GetTransform()->GetRotation().y);
    const float frontZ = cosf(GetTransform()->GetRotation().y);

    // ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��όv�Z����
    const float dot = (frontX * vx) + (frontZ * vz);

    // ��]�p�������ȏꍇ�͉�]���s��Ȃ�
    const float angle = acosf(dot); // ���W�A��
    //if (fabsf(angle) <= 0.001f) return;
    if (fabsf(angle) <= 0.01f) return;

    // ���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����  
    NO_CONST float rot = 1.0f - dot;   // �␳�l                    
    if (rot > turnSpeed) rot = turnSpeed;

    // ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    const float cross = (frontZ * vx) - (frontX * vz);

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    GetTransform()->AddRotationY((cross < 0.0f) ? -rot : rot);
}