#pragma once

#include "../Mame/Graphics/Model.h"

#ifdef USE_IMGUI
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_internal.h"
#include "../../External/imgui/imgui_impl_dx11.h"
#include "../../External/imgui/imgui_impl_win32.h"
#endif

#include "../GeometricPrimitive.h"
#include "../sprite.h"

#include <DirectXMath.h>

#include "Common.h"

#include "Collision.h"

#include "Terrain/Terrain.h"

class Character
{
public:
    Character();
    virtual ~Character() {}

    virtual void Render(const float& elapsedTime);  // �`�揈��
    virtual void DrawDebug();                       // �f�o�b�O�`��

    void UpdateAABB();                              // AABB�X�V����


public:
    // �_���[�W��^����
    virtual bool ApplyDamage(
        const int& damage, 
        const float& invincibleTime
    );

protected:
    // �ړ�����
    void Move(const float& vx, const float& moveSpeed);

    // ���񏈗�
    void Turn(
        const float& elapsedTime, 
        NO_CONST float vx, 
        NO_CONST float turnSpeed
    );

    // �W�����v����
    void Jump(const float& jumpSpeed);

protected:
    void UpdateVelocity(const float& elapsedTime);              // ���͏����X�V����
    void UpdateInvincibleTimer(const float& elapsedTime);       // ���G���ԍX�V����

protected:
    virtual void OnLanding()  {}                             // ���n�����Ƃ��ɌĂ΂��  
    virtual void OnDash()     {}                             // �_�b�V�����Ă���Ƃ��ɌĂ΂��
    virtual void OnBounce()   {}                             // �o�E���X����Ƃ��ɌĂ΂��
    virtual void OnDamaged()  {}                             // �_���[�W���󂯂��Ƃ��ɌĂ΂��  
    virtual void OnDead()     {}                             // ���S�����Ƃ��ɌĂ΂��
    virtual void OnFallDead() {}                             // �������E�����~�X�����Ƃ��ɌĂ΂��
    virtual void OnHitWall()  {}                             // �ǂɓ����������̏���

private: 
    void UpdateVerticalVelocity(const float& elapsedFrame);     // �������͍X�V����
    void UpdateVerticalMove(const float& elapsedTime);          // �����ړ��X�V����

    void VerticalFall(const float& fallSpeed);                  // ������������(�R�[�h�������Ȃ����̂ŗ������̏�����������Ɉړ�)
    void VerticalRise(const float& riseSpeed);                  // �����㏸����(�R�[�h�������Ȃ����̂ŏ㏸���̏�����������Ɉړ�)

    void UpdateHorizontalVelocity(const float& elapsedFrame);   // �������͍X�V����
    void UpdateHorizontalMove(const float& elapsedTime);        // �����ړ��X�V����

    void HorizontalRightLeft(NO_CONST float horizontalSpeed);     // �����ړ�����(�R�[�h�������Ȃ����̂Ő����ړ����̏�����������Ɉړ�)


public: // �A�j���[�V���������֐��֘A�i�V���[�g�J�b�g�֐��j

    // �A�j���[�V�����Đ��ݒ�
    void PlayAnimation(
        const int& index,
        const bool& loop,
        const float& speed = 1.0f,
        const float& blendSeconds = 1.0f)
    {
        model->PlayAnimation(index, loop, speed, blendSeconds);
    }

    // �A�j���[�V�����X�V����
    void UpdateAnimation(const float& elapsedTime) { model->UpdateAnimation(elapsedTime); }

    // �A�j���[�V�������Đ������ǂ���
    bool IsPlayAnimation() const { return model->IsPlayAnimation(); }

    // �A�j���[�V�����Đ����x�ݒ�i�r���ōĐ����x��ς������Ƃ��ȂǂɁj
    void SetAnimationSpeed(const float& speed) { model->SetAnimationSpeed(speed); }

public: // �擾�E�ݒ�֐��֘A

    // �g�����X�t�H�[���擾
    Transform* GetTransform() const { return model->GetTransform(); }

    // �J���[�ݒ�
    const DirectX::XMFLOAT4& SetMaterialColor() const { return materialColor; }
    void SetMaterialColor(const DirectX::XMFLOAT4& color) { materialColor = color; }

    // ���x�̐ݒ�
    void SetVelocityX(const float velocityX) { velocity.x = velocityX; }
    void SetVelocityY(const float velocityY) { velocity.y = velocityY; }
    void SetVelocityZ(const float velocityZ) { velocity.z = velocityZ; }

    // ���f���J���[�̎擾�E�ݒ�
    const DirectX::XMFLOAT4& GetModelColor() const { return modelColor; }
    const float GetModelColorAlpha(const float colorAlpha) { modelColor.w = colorAlpha; }
    void SetModelColorAlpha(const float colorAlpha) { modelColor.w = colorAlpha; }

    // �ۑ������ړ������x�N�g���̎擾
    const float GetSaveMoveVecX() const { return saveMoveVecX_; }

    // �ړ����x�̎擾�E�ݒ�
    const float GetMoveSpeed() const { return moveSpeed_; }
    void SetMoveSpeed(const float speed) { moveSpeed_ = speed; }    
    
    // �����x�̎擾�E�ݒ�
    const float GetAcceleration() const { return acceleration_; }
    void SetAcceleration(const float acceleration) { acceleration_ = acceleration; }    
    
    // ���C�̎擾�E�ݒ�
    const float GetFriction() const { return friction_; }
    void SetFriction(const float friction) { friction_ = friction; }

    // ���񑬓x�̎擾�E�ݒ�
    const float GetTurnSpeed() const { return turnSpeed_; }
    void SetTurnSpeed(const float turnSpeed) { turnSpeed_ = turnSpeed; }

    // �W�����v���x�̎擾�E�ݒ�
    const float GetJumpSpeed() const { return jumpSpeed_; }
    void SetJumpSpeed(const float jumpSpeed) { jumpSpeed_ = jumpSpeed; }


    // �̗͂̎擾
    const int& GetHealth() const { return health; }

    // �n�ʂɂ��Ă��邩�ǂ����̎擾�E�ݒ�
    const bool& GetIsGround() const { return isGround_; }
    void SetIsGround(const bool isGround) { isGround_ = isGround; }

    // ���G���ǂ����̎擾�E�ݒ�
    const bool GetIsInvincible() const { return isInvincible; }
    void SetIsInvincible(const bool invincible) { isInvincible = invincible; }

    // AABB�ݒ�(�����蔻��̃T�C�Y��r���ŕς������Ƃ��Ȃǂ�)
    void SetAABB(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);

public:
    std::unique_ptr<Model> model    =  nullptr;                     // ���f��

    std::unique_ptr<GeometricPrimitive> geometricAABB_ = nullptr;   // �����蔻��AABB�`��p�W�I���g���b�N�v���~�e�B�u
    Collision::AABB aabb_                   = {};                   // �����蔻��AABB
    Collision::AABB lastLandingTerrainAABB_ = {};                   // �Ō�ɒ��n�����n�`��AABB�i�����~�X�̕������Ɏg�p�j
    Terrain*        saveTerrain_            = nullptr;              // �n�`��ۑ�����|�C���^
    Terrain*        saveWall_               = nullptr;              // �ǂ�AABB��ۑ�����AABB

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders;

    DirectX::XMFLOAT4 modelColor    =   { 1.0f, 1.0f, 1.0f, 1.0f }; // ���f���J���[
    DirectX::XMFLOAT4 materialColor =   { 1.0f, 0.0f, 0.0f, 0.4f }; // �}�e���A���J���[

    DirectX::XMFLOAT3 velocity      =   { 0,0,0 };                  // ���x
         
    float       stepOffset          =   1.0f;                       // �ʒu�␳(Y�ʒu���L�����N�^�[�̒��S�ɂȂ�悤�ɒ���)
                                        
    float       moveVecX_           =   0.0f;                       // �ړ��x�N�g��X
    float       saveMoveVecX_        =   1.0f;                       // �ړ��x�N�g����ۑ�����x�N�g��X�i�ŏ��͉E���������Ă����j

    float       defaultAcceleration =   1.0f;                       // �����͂̏����l
    float       acceleration_       =   defaultAcceleration;        // ������
    float       defaultGravity      =  -1.0f;                       // �d�͏����l
    float       gravity             =   defaultGravity;             // �d��
    float       defaultFriction     =   0.5f;                       // ���C�͂̏����l
    float       friction_           =   0.5f;                       // ���C��
    float       airControl          =   0.3f;                       // �󒆐���

    float       defaultMoveSpeed    =  5.0f;                        // �ړ����x�����l
    float       moveSpeed_          =  defaultMoveSpeed;            // �ړ����x(�ő�ړ����x�ɑ�������)
    float       turnSpeed_          =  ToRadian(900.0f);            // ���񑬓x(180.f * 5)
    float       jumpSpeed_           = 10.0f;                       // �W�����v���x
    float       maxMoveSpeed        =  5.0f;                        // �ő�ړ����x

    float       slopeRate           =   1.0f;                       // �X�Η�
                                        
    float       invincibleTimer     =   0.0f;                       // ���G����

    float       defaultJumpTime     =   0.3f;                       // �W�����v����
    float       jumpTimer           =   0.0f;                       // �W�����v�^�C�}�[

    int         health              =   3;                          // �̗�
    int         jumpCount           =   0;                          // �W�����v��
    int         jumpLimit           =   1;                          // �ő�W�����v��
                                       
    bool        isGround_            =   false;                      // �n�ʂɂ��Ă��邩
    bool        isHitWall_          =   false;                      // �ǂɓ������Ă��邩
    bool        isBounce            =   false;                      // �o�E���X�����邩
    bool        isDash              =   false;                      // �_�b�V�����Ă��邩
    bool        isInvincible        =   false;                      // ���G���ǂ���(�{�X�Ɏg��)

private:
    // �傫���̕ύX������Ƃ��ɓ����蔻���AABB�`��̑傫�����ύX�����邽�߂�private�ɂ��Ă���
    DirectX::XMFLOAT3 defaultMin_   = { -0.4f, -0.0f, -0.4f };       // �����蔻���min�f�t�H���g�l
    DirectX::XMFLOAT3 defaultMax_   = { +0.4f, +0.8f, +0.4f };       // �����蔻���max�f�t�H���g�l
};

