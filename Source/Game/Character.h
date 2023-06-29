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

class Character
{
public:
    Character();
    virtual ~Character() {}

    virtual void Render(const float& elapsedTime);  // �`�揈��
    virtual void DrawDebug();                       // �f�o�b�O�`��

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
    void UpdateAABB();                                      // AABB�X�V����
    void UpdateVelocity(const float& elapsedTime);          // ���͏����X�V����
    void UpdateInvincibleTimer(const float& elapsedTime);   // ���G���ԍX�V����

protected:
    virtual void OnLanding() {}                             // ���n�����Ƃ��ɌĂ΂��  
    virtual void OnDash()    {}                             // �_�b�V�����Ă���Ƃ��ɌĂ΂��
    virtual void OnBounce()  {}                             // �o�E���X����Ƃ��ɌĂ΂��
    virtual void OnDamaged() {}                             // �_���[�W���󂯂��Ƃ��ɌĂ΂��  
    virtual void OnDead()    {}                             // ���S�����Ƃ��ɌĂ΂��

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
    void SetMaterialColor(const DirectX::XMFLOAT4& color) { materialColor = color; }

    // ���G���ǂ����̐ݒ�E�擾
    const float& GetIsInvincible() const { return isInvincible; }
    void SetIsInvincible(const float& invincible) { isInvincible = invincible; }

    // AABB�Đݒ�(�����蔻��̃T�C�Y��r���ŕς������Ƃ��Ȃǂ�)
    void ResetAABB(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);

public:
    std::unique_ptr<Model> model    =  nullptr;                     // ���f��

    std::unique_ptr<GeometricPrimitive> geometricAABB_ = nullptr;   // �����蔻��`��p�W�I���g���b�N�v���~�e�B�u
    Collision::AABB aabb_           = {};                           // �����蔻��\����

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders;

    DirectX::XMFLOAT4 materialColor =   { 1.0f, 0.0f, 0.0f, 0.4f }; // �}�e���A���J���[

    DirectX::XMFLOAT3 velocity      =   { 0,0,0 };                  // ���x
           
    float       modelColorAlpha     =   1.0f;

    float       stepOffset          =   1.0f;                       // �ʒu�␳(Y�ʒu���L�����N�^�[�̒��S�ɂȂ�悤�ɒ���)
                                        
    float       moveVecX            =   0.0f;                       // �ړ��x�N�g��X
    float       saveMoveVecX        =   1.0f;                       // �ړ��x�N�g����ۑ�����x�N�g��X�i�ŏ��͉E���������Ă����j

    float       defaultAcceleration =   1.0f;                       // �����͂̏����l
    float       acceleration        =   defaultAcceleration;        // ������
    float       defaultGravity      =  -1.0f;                       // �d�͏����l
    float       gravity             =   defaultGravity;             // �d��
    float       defaultFriction     =   0.5f;                       // ���C�͂̏����l
    float       friction            =   0.5f;                       // ���C��
    float       airControl          =   0.3f;                       // �󒆐���

    float       defaultMoveSpeed    =  5.0f;                        // �ړ����x�����l
    float       moveSpeed           =  defaultMoveSpeed;            // �ړ����x(�ő�ړ����x�ɑ�������)
    float       turnSpeed           =  ToRadian(900.0f);            // ���񑬓x(180.f * 5)
    float       jumpSpeed           =  10.0f;                       // �W�����v���x
    float       maxMoveSpeed        =  5.0f;                        // �ő�ړ����x

    float       slopeRate           =   1.0f;                       // �X�Η�
                                        
    float       invincibleTimer     =   0.0f;                       // ���G����

    float       defaultJumpTime     =   0.3f;                       // �W�����v����
    float       jumpTimer           =   0.0f;                       // �W�����v�^�C�}�[

    int         health              =   3;                          // �̗�
    int         jumpCount           =   0;                          // �W�����v��
    int         jumpLimit           =   1;                          // �ő�W�����v��
                                       
    bool        isGround            =   false;                      // �n�ʂɂ��Ă��邩
    bool        isBounce            =   false;                      // �o�E���X�����邩
    bool        isDash              =   false;                      // �_�b�V�����Ă��邩
    bool        isInvincible        =   false;                      // ���G���ǂ���(�{�X�Ɏg��)

private:
    // �傫���̕ύX������Ƃ��ɓ����蔻���AABB�`��̑傫�����ύX�����邽�߂�private�ɂ��Ă���
    DirectX::XMFLOAT3 defaultMin_   = { -0.4f, -0.0f, -0.4f };       // �����蔻���min�f�t�H���g�l
    DirectX::XMFLOAT3 defaultMax_   = { +0.4f, +0.8f, +0.4f };       // �����蔻���max�f�t�H���g�l
};

