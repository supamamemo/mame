#include "Character.h"
#include "Collision.h"
#include "Mathf.h"
#include "../Mame/Graphics/Graphics.h"
#include "OperatorXMFLOAT3.h"
#include "../misc.h"

Character::Character()
{
    Graphics& graphics = Graphics::Instance();

    debugModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/cube.fbx", true);

    // �����蔻��T�C�Y�ݒ�   
    {
        //debugModel->skinned_meshes.boundingBox[0] = { -0.5f, -0.5f, -0.5f };
        //debugModel->skinned_meshes.boundingBox[1] = {  0.5f,  0.5f,  0.5f };
        aabb.min.x = -40.0f;
        aabb.min.y =  0.0f;
        aabb.min.z = -40.0f;

        aabb.max.x =  40.0f;
        aabb.max.y =  80.0f;
        aabb.max.z =  40.0f;
    }

    geometricAABB_ = std::make_unique<GeometricAABB>(graphics.GetDevice(), aabb.min, aabb.max);
}


void Character::Render(const float& /*elapsedTime*/)
{
    Graphics& graphics = Graphics::Instance();

    // world�s��X�V
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��
    if (&model->keyframe)
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, 1), &model->keyframe);
    }
    else
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
    }


#if _DEBUG

    // �f�o�b�O���f���ł̕`��
#if 0
    DirectX::XMFLOAT4X4 debugTransform = {};

    // ���[���h�s��̎擾�ƃX�P�[������
    DirectX::XMStoreFloat4x4(&debugTransform, debugModel->GetTransform()->CalcWorldMatrix(0.01f));

    // ���[���h�s��ݒ�
    debugTransform = SetDebugModelTransform(debugTransform);

    // �`��
    debugModel->skinned_meshes.render(graphics.GetDeviceContext(), debugTransform, { 1.0f, 0.0f, 0.0f, 0.2f }, nullptr);
#endif

    // ���X�^���C�U�X�e�[�g�쐬�E�ݒ�
#if 0
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state = nullptr;
    
    // ���X�^���C�U�X�e�[�g�쐬
    NO_CONST HRESULT hr = {};
    NO_CONST D3D11_RASTERIZER_DESC rasterizer_desc = {};
    //rasterizer_desc.FillMode                = D3D11_FILL_WIREFRAME; // �h��Ԃ�
    rasterizer_desc.FillMode                = D3D11_FILL_SOLID; // �h��Ԃ�
    //rasterizer_desc.CullMode                = D3D11_CULL_BACK;	    // �w�ʃJ�����O�i���ʂ�`�悵�Ȃ��j
    rasterizer_desc.CullMode                = D3D11_CULL_NONE;	    // �J�����O�Ȃ�

    // ?
    //rasterizer_desc.FrontCounterClockwise   = FALSE;			    // �O�p�`���𔽎��v���i�����j�ɂ��邩
    rasterizer_desc.FrontCounterClockwise   = TRUE;			    // �O�p�`���𔽎��v���i�����j�ɂ��邩

    rasterizer_desc.DepthBias               = 0;	                // �w�肳�ꂽ�s�N�Z���ɒǉ����ꂽ�[�x�l
    rasterizer_desc.DepthBiasClamp          = 0;	                // �s�N�Z���̍ő�[�x�o�C�A�X
    rasterizer_desc.SlopeScaledDepthBias    = 0;	                // �w�肳�ꂽ�s�N�Z���̌X���̃X�J���[
    rasterizer_desc.DepthClipEnable         = TRUE;	                // �����ɂ��N���b�s���O���s�����̃t���O
    rasterizer_desc.ScissorEnable           = FALSE;                // �V�U�[��`�J�����O���s�����̃t���O
    rasterizer_desc.MultisampleEnable       = FALSE;                // �}���`�T���v�����O�A���`�G�C���A�X(���́FMSAA)�̃����_�[�^�[�Q�b�g���g�p���Ă��鎞�A�l�ӌ`���C���A���`�G�C���A�X���s�����A�A���t�@���C���A���`�G�C���A�X���s���������߂�t���O
    rasterizer_desc.AntialiasedLineEnable   = FALSE;                // MSAA�̃����_�[�^�[�Q�b�g���g�p���Ă��鎞�A�����`���MultisampleEnable��false�̂Ƃ��A�A���`�G�C���A�X��L���ɂ��܂�
    hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizer_state.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    // ���X�^���C�U�X�e�[�g�ݒ�(���C���[�t���[��)
    graphics.GetDeviceContext()->RSSetState(rasterizer_state.Get());

#else
    // RS�ԍ�
    {
        // 0 �\���b�h�E���J�����O
        // 1 ���C���[�t���[���E���J�����O
        // 2 ���C���[�t���[���E�J�����O�Ȃ�
        // 3 �\���b�h�E�J�����O�Ȃ�
    }

    // ���X�^���C�U�ݒ�(�\���b�h�E�J�����O�Ȃ�)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 3, 0, 0);

#endif

    // ��]�Ȃ����[���h�s��̍쐬
    NO_CONST  DirectX::XMFLOAT4X4 noRotationTransform = {};
    {
        const DirectX::XMFLOAT3& scale      = GetTransform()->GetScale();
        const DirectX::XMFLOAT3& position   = GetTransform()->GetPosition();
        const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
        const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMStoreFloat4x4(&noRotationTransform, S * T);
    }

    // �J���[�ݒ�
    const DirectX::XMFLOAT4 materialColor = { 1, 0, 0, 0.4f };
    
    // AABB�`��
    geometricAABB_->render(graphics.GetDeviceContext(), noRotationTransform, materialColor);

    // ���X�^���C�U�Đݒ�(�\���b�h�E���J�����O)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 0, 0, 0);

#endif // _DEBUG
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


void Character::UpdateAABB(const float& /*elapsedTime*/)
{
    aabb.position = GetTransform()->GetPosition();
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

DirectX::XMFLOAT4X4 Character::SetDebugModelTransform(DirectX::XMFLOAT4X4 transoform)
{
    // 0: Target model
    // 1: Bounding box model
//    DirectX::XMFLOAT3 dimensions[] = {
//#if 1
//            {
//                model->skinned_meshes.boundingBox[1].x - model->skinned_meshes.boundingBox[0].x,
//                model->skinned_meshes.boundingBox[1].y - model->skinned_meshes.boundingBox[0].y,
//                model->skinned_meshes.boundingBox[1].z - model->skinned_meshes.boundingBox[0].z,
//            },
//#else
//            { 100.0f, 150.0f, 60.0f },
//#endif
//            {
//                debugModel->skinned_meshes.boundingBox[1].x - debugModel->skinned_meshes.boundingBox[0].x,
//                debugModel->skinned_meshes.boundingBox[1].y - debugModel->skinned_meshes.boundingBox[0].y,
//                debugModel->skinned_meshes.boundingBox[1].z - debugModel->skinned_meshes.boundingBox[0].z,
//            },
//    };
//    DirectX::XMFLOAT3 centers[] = {
//        {
//            model->skinned_meshes.boundingBox[0].x + (model->skinned_meshes.boundingBox[1].x - model->skinned_meshes.boundingBox[0].x) * 0.5f,
//            model->skinned_meshes.boundingBox[0].y + (model->skinned_meshes.boundingBox[1].y - model->skinned_meshes.boundingBox[0].y) * 0.5f,
//            model->skinned_meshes.boundingBox[0].z + (model->skinned_meshes.boundingBox[1].z - model->skinned_meshes.boundingBox[0].z) * 0.5f,
//        },
//        {
//            debugModel->skinned_meshes.boundingBox[0].x + (debugModel->skinned_meshes.boundingBox[1].x - debugModel->skinned_meshes.boundingBox[0].x) * 0.5f,
//            debugModel->skinned_meshes.boundingBox[0].y + (debugModel->skinned_meshes.boundingBox[1].y - debugModel->skinned_meshes.boundingBox[0].y) * 0.5f,
//            debugModel->skinned_meshes.boundingBox[0].z + (debugModel->skinned_meshes.boundingBox[1].z - debugModel->skinned_meshes.boundingBox[0].z) * 0.5f,
//        },
//    };

    // min����max�܂ł̑傫��(�����H)�����߂�
    // (max - min) = (max - (-min)) = (max + (+min))
    NO_CONST DirectX::XMFLOAT3 min       = debugModel->skinned_meshes.boundingBox[0];
    NO_CONST DirectX::XMFLOAT3 max       = debugModel->skinned_meshes.boundingBox[1];
    const    DirectX::XMFLOAT3 dimension = max - min;
    const    DirectX::XMMATRIX S         = DirectX::XMMatrixScaling(dimension.x, dimension.y, dimension.z);

    //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(dimensions[0].x / dimensions[1].x, dimensions[0].y / dimensions[1].y, dimensions[0].z / dimensions[1].z);
    //DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(centers[0].x - centers[1].x, centers[0].y - centers[1].y, centers[0].z - centers[1].z);

    const DirectX::XMFLOAT3& position = debugModel->GetTransform()->GetPosition();
    const DirectX::XMMATRIX  T        = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    //
    DirectX::XMFLOAT4X4 world = {};
    //DirectX::XMStoreFloat4x4(&world, S * DirectX::XMLoadFloat4x4(&transoform));
    DirectX::XMStoreFloat4x4(&world, S * T);

    return world;
}
