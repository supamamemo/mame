#include "Character.h"

#include "../Mame/Graphics/Graphics.h"
#include "../misc.h"

#include "OperatorXMFLOAT3.h"
#include "Mathf.h"
#include "Collision.h"
#include "Terrain/TerrainManager.h"

Character::Character()
{
    Graphics& graphics = Graphics::Instance();

    create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/wireframe.cso", pixel_shaders.GetAddressOf());

    geometricAABB_ = std::make_unique<GeometricAABB>(graphics.GetDevice(), defaultMin_, defaultMax_);
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
        model->skinned_meshes->render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, modelColorAlpha), &model->keyframe);
    }
    else
    {
        model->skinned_meshes->render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, modelColorAlpha), nullptr);
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
    debugModel->skinned_meshes->render(graphics.GetDeviceContext(), debugTransform, { 1.0f, 0.0f, 0.0f, 0.2f }, nullptr);
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
        const DirectX::XMFLOAT3  scale      = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
        const DirectX::XMFLOAT3& position   = GetTransform()->GetPosition();
        const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
        const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMStoreFloat4x4(&noRotationTransform, S * T);
    }

    // �J���[�ݒ�
    //const DirectX::XMFLOAT4 materialColor = { 1, 0, 0, 0.4f };
    
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
}


// �_���[�W��^����
bool Character::ApplyDamage(const int& damage, const float& invincibleTime)
{
    // �_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage <= 0) return false;

    // ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0) return false;

    // ���G���Ԃ��c���Ă����猒�N��Ԃ�ύX���Ȃ�
    if (invincibleTimer > 0.0f) return false;

    // ���G���[�h�Ȃ猒�N��Ԃ�ύX���Ȃ�
    if (isInvincible) return false;

    // �_���[�W����
    const int damagedHealth = health - damage;
    health = (damagedHealth > 0) ? damagedHealth : 0;

    // ���G���Ԃ�ݒ�
    invincibleTimer = invincibleTime;

    // ���S�ʒm
    if (health <= 0) OnDead();
    // �_���[�W�ʒm
    else OnDamaged();

    // ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
    return true;
}


// �ړ�����
void Character::Move(
    const float& vx, 
    const float& moveSpeed)
{
    // �ړ������x�N�g����ݒ�
    moveVecX_ = vx;

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


// AABB�X�V����
void Character::UpdateAABB()
{
    const DirectX::XMFLOAT3& position = GetTransform()->GetPosition();

    aabb_.min = position + defaultMin_;
    aabb_.max = position + defaultMax_;
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
    const float gravityMax = -20.0f;
    if (velocity.y < gravityMax) velocity.y = gravityMax;
}

// �����ړ��X�V����
void Character::UpdateVerticalMove(const float& elapsedTime)
{
    NO_CONST Transform* transform = GetTransform();

    // ���������̈ړ���
    const float my = velocity.y * elapsedTime;

    // ������
    if (my < 0.0f)
    {
        // ������������
        VerticalFall(my);
    }
    // �㏸��
    else if (my > 0.0f)
    {
        // �����㏸����
        VerticalRise(my);
    }
}


// ������������
void Character::VerticalFall(const float& fallSpeed)
{
    // �������Ă��邩�ǂ���
    NO_CONST bool isHitY = false;

    // �n�`�ɓ������Ă��邩�Փ˔��菈�����s��
    {
        NO_CONST TerrainManager& terrainManager = TerrainManager::Instance();
        const int terrainCount = terrainManager.GetTerrainCount();
        for (int i = 0; i < terrainCount; ++i)
        {
            const Terrain* terrain = terrainManager.GetTerrain(i);

            if (Collision::IntersectAABBVsAABB(this->aabb_, terrain->aabb_))
            {
                // �㉺���E�ɂ��ꂼ��d�Ȃ��Ă���l�����߂�i�G�l�~�[�̓�����v���C���[�̑����܂ł̋����j
                NO_CONST float vec = 0.0f;
                vec                         = aabb_.min.y - terrain->aabb_.max.y;
                const float overlapUp       = sqrtf(vec * vec); // �ォ��߂荞��ł���
                vec                         = aabb_.max.y - terrain->aabb_.min.y;
                const float overlapBottom   = sqrtf(vec * vec); // ������߂荞��ł���
                vec                         = aabb_.min.x - terrain->aabb_.max.x;
                const float overlapRight    = sqrtf(vec * vec); // �E����߂荞��ł���
                vec                         = aabb_.max.x - terrain->aabb_.min.x;
                const float overlapLeft     = sqrtf(vec * vec); // ������߂荞��ł���
                // ��ԏ�����(�d�Ȃ��Ă��Ȃ�)�l�����߂�(
                const float overlapY        = (std::min)(overlapUp, overlapBottom);
                const float overlapX        = (std::min)(overlapRight, overlapLeft);

                // X���̕����d�Ȃ肪������(�߂肱��ł���)�Ȃ�continue
                if (overlapX < overlapY) continue;

                // ��ւ̏d�Ȃ肪�Ȃ���Έʒu���C�����Ȃ�
                if (overlapUp == 0.0f)
                {
                    isHitY     = true; // �������Ă���         
                    velocity.y = 0.0f; // Y���x�����Z�b�g

                    // ���n�����n�`AABB��min�Emax��X�ʒu�Emax��Y�ʒu��ۑ�
                    lastLandingTerrainAABBMinX = terrain->aabb_.min.x;
                    lastLandingTerrainAABBMaxX = terrain->aabb_.max.x;
                    lastLandingTerrainAABBMaxY = terrain->aabb_.max.y;

                    // �o�E���X���Ȃ�o�E���X������
                    // �o�E���X�񐔂����[�v���ŏ�����Ȃ����߂�break���Ă���
                    if (isBounce)
                    {
                        OnBounce();
                        break;
                    }

                    continue;
                }

                // �����ォ��̏d�Ȃ�̕�����������Ώォ��߂荞��ł���Ɣ��f���A
                // �d�Ȃ��Ă��镪���������߂�
                // ���ォ��̂߂荞�݂�D�悵��<=�ɂ��Ă���
                if (overlapUp <= overlapBottom)
                {
                    isHitY      = true; // �������Ă���   
                    velocity.y  = 0.0f; // Y���x�����Z�b�g

                    // �d�Ȃ��Ă��镪�����ʒu���C��
                    GetTransform()->AddPositionY(overlapUp);

                    // �����߂����AABB�̍ŏ����W�ƍő���W���X�V
                    UpdateAABB();

                    // ���n�����n�`AABB��min�Emax��X�ʒu�Emax��Y�ʒu��ۑ�
                    lastLandingTerrainAABBMinX = terrain->aabb_.min.x;
                    lastLandingTerrainAABBMaxX = terrain->aabb_.max.x;
                    lastLandingTerrainAABBMaxY = terrain->aabb_.max.y;

                    // �o�E���X���͒��˂�����
                    if (isBounce)
                    {
                        OnBounce();
                        break;
                    }
                    else
                    {
                        // ���n����
                        if (!isGround) OnLanding();
                        isGround = true;
                        break;
                    }
                }
            }
        }
    }

    // �n�`�ɓ������Ă��Ȃ���Η���������
    if (!isHitY)
    {
        // �󒆂ɕ����Ă���
        GetTransform()->AddPositionY(fallSpeed);
        isGround = false;

        // ���ɗ������痎�����E�����~�X�����Ƃ��̏������s��
        if (GetTransform()->GetPosition().y < -15.0f)
        {
            OnFallDead(); 
        }
    }
}

// �����㏸����
void Character::VerticalRise(const float& riseSpeed)
{
    // ��ɏ㏸���x���ʒu�ɉ��Z���Ă���(�n�ʂƏՓ˔��肳���Ȃ�����)
    GetTransform()->AddPositionY(riseSpeed);

    // AABB�X�V
    UpdateAABB(); 

    // �󒆂ɂ���
    isGround = false;

    // �n�`�ɓ������Ă��邩�Փ˔��菈�����s��
    {
        NO_CONST TerrainManager& terrainManager = TerrainManager::Instance();
        const int terrainCount = terrainManager.GetTerrainCount();
        for (int i = 0; i < terrainCount; ++i)
        {
            const Terrain* terrain = terrainManager.GetTerrain(i);

            if (Collision::IntersectAABBVsAABB(aabb_, terrain->aabb_))
            {
                // �㉺���E�̂��ꂼ��d�Ȃ��Ă���l�����߂�
                NO_CONST float vec = 0.0f;
                vec                         = aabb_.min.y - terrain->aabb_.max.y;
                const float overlapUp       = sqrtf(vec * vec); // �ォ��߂荞��ł���
                vec                         = aabb_.max.y - terrain->aabb_.min.y;
                const float overlapBottom   = sqrtf(vec * vec); // ������߂荞��ł���
                vec                         = aabb_.min.x - terrain->aabb_.max.x;
                const float overlapRight    = sqrtf(vec * vec); // �E����߂荞��ł���
                vec                         = aabb_.max.x - terrain->aabb_.min.x;
                const float overlapLeft     = sqrtf(vec * vec); // ������߂荞��ł���
                // ��ԏ�����(�d�Ȃ��Ă��Ȃ�)�l�����߂�
                const float overlapY        = (std::min)(overlapUp, overlapBottom);
                const float overlapX        = (std::min)(overlapRight, overlapLeft);

                // X���̕����d�Ȃ肪������(�߂肱��ł���)�Ȃ�continue
                if (overlapX < overlapY) continue;

                // ���艺����̏d�Ȃ�̕�����������Ή�����߂荞��ł���Ɣ��f���A
                // �d�Ȃ��Ă��镪���������߂�
                if (overlapBottom < overlapUp)
                {
                    velocity.y = 0.0f; // Y���x�����Z�b�g

                    // �d�Ȃ��Ă��镪�����ʒu���C��
                    GetTransform()->AddPositionY(-overlapBottom);

                    // �����߂����AABB�̍ŏ����W�ƍő���W���X�V
                    UpdateAABB();

                    break;
                }
            }
        }
    }
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
        const float moveVecDist = sqrtf(moveVecX_ * moveVecX_);
        if (moveVecDist > 0.0f)
        {
            // ������
            float acceleration = this->acceleration * elapsedFrame;

            // �󒆂ɂ���Ƃ��͉����͂����炷
            if (!isGround) acceleration *= airControl;

            // �ړ��x�N�g���ɂ���������
            velocity.x += moveVecX_ * acceleration;

            // �ő呬�x����
            const float dist = sqrtf(velocity.x * velocity.x);
            if (dist > maxMoveSpeed)
            {
                velocity.x = moveVecX_ * maxMoveSpeed;
            }

            //// �����ŃK�^�K�^���Ȃ��悤�ɂ���
            //if (isGround && slopeRate > 0.0f) // ���n���̓X���[�v�Ή�
            //{
            //    velocity.y -= dist * slopeRate * elapsedFrame;
            //}
        }
    }

    // �ړ��x�N�g�������Z�b�g
    moveVecX_ = 0.0f;
}

// �����ړ��X�V����
void Character::UpdateHorizontalMove(const float& elapsedTime)
{
    // �������͗ʌv�Z
    const float velocityLengthX = sqrtf(velocity.x * velocity.x);

    if (velocityLengthX > 0.0f)
    {
        // �����ړ��l
        const float mx = velocity.x * elapsedTime;

        HorizontalRightLeft(mx);
    }

}


// �����ړ�����
void Character::HorizontalRightLeft(NO_CONST float horizontalSpeed)
{
    // �������Ă��邩�ǂ���
    NO_CONST bool isHitX = false;

    // �n�`�ɓ������Ă��邩�Փ˔��菈�����s��
    {
        NO_CONST TerrainManager& terrainManager = TerrainManager::Instance();
        const int terrainCount = terrainManager.GetTerrainCount();
        for (int i = 0; i < terrainCount; ++i)
        {
            const Terrain* terrain = terrainManager.GetTerrain(i);

            if (Collision::IntersectAABBVsAABB(aabb_, terrain->aabb_))
            {
                // �㉺���E�̂��ꂼ��d�Ȃ��Ă���l�����߂�
                NO_CONST float vec = 0.0f;
                vec                         = aabb_.min.y - terrain->aabb_.max.y;
                const float overlapUp       = sqrtf(vec * vec); // �ォ��߂荞��ł���
                vec                         = aabb_.max.y - terrain->aabb_.min.y;
                const float overlapBottom   = sqrtf(vec * vec); // ������߂荞��ł���
                vec                         = aabb_.min.x - terrain->aabb_.max.x;
                const float overlapRight    = sqrtf(vec * vec); // �E����߂荞��ł���
                vec                         = aabb_.max.x - terrain->aabb_.min.x;
                const float overlapLeft     = sqrtf(vec * vec); // ������߂荞��ł���
                // ��ԏd�Ȃ��Ă��Ȃ��l�����߂�
                const float overlapY        = (std::min)(overlapUp, overlapBottom);
                const float overlapX        = (std::min)(overlapRight, overlapLeft);

                // Y���̕����d�Ȃ肪������(�߂肱��ł���)�Ȃ�continue
                if (overlapY < overlapX) continue;

                // ���ւ̏d�Ȃ肪�Ȃ���ΏC�����Ȃ�
                if (overlapX == 0.0f) continue;

                // �o�E���X���Ȃ�ǂɓ�����������X���x�𔽓]������
                if (isBounce)
                {                  
                    horizontalSpeed = -horizontalSpeed; // ����̑��x�𔽓]
                    velocity.x      = -velocity.x;      // ���˕Ԃ��ċ󒆂ɕ����Ă�Ƃ���X���x�����]������
                    saveMoveVecX    = -saveMoveVecX;    // �����Ă���������t�ɂ�����
                }
                else
                {
                    velocity.x  = 0.0f;  // X���x�����Z�b�g
                    isHitX      = true;  // �������Ă���   
                }

                // �������E����̏d�Ȃ�̕�����������ΉE����߂荞��ł��āA
                // �t�ɉE��荶����̏d�Ȃ�̕�����������΍�����߂荞��ł���Ɣ��f���A
                // �d�Ȃ��Ă��镪�����ʒu�������߂�
                // ���E����̂߂荞�݂�D�悵��<=�ɂ��Ă���
                if (overlapRight <= overlapLeft)
                {
                    GetTransform()->AddPositionX(overlapRight);
                }
                else if (overlapLeft < overlapRight)
                {
                    GetTransform()->AddPositionX(-overlapLeft);
                }

                // �����߂����AABB�̍ŏ����W�ƍő���W���X�V
                UpdateAABB();

                break;
            }
        }
    }
    
    // �n�`�ɏՓ˂��Ă��Ȃ���Έʒu��X���x�����Z
    if (!isHitX || isBounce)
    {
        // �ړ�����
        GetTransform()->AddPositionX(horizontalSpeed);
    }
}


void Character::ResetAABB(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
    // min�Emax�̍Đݒ�
    defaultMin_ = min;
    defaultMax_ = max;

    // �����蔻���AABB�`����Đݒ�
    ID3D11Device* device = Graphics::Instance().GetDevice();
    geometricAABB_ = std::make_unique<GeometricAABB>(device, defaultMin_, defaultMax_);
}
