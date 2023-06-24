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

    virtual void Render(const float& elapsedTime); // �`�揈��

    virtual void DrawDebug();           // �f�o�b�O�`��

public: // �擾�E�ݒ�֐��֘A
    // ����̓e�X�g�p�Ŏg���Ă邾���Ȃ̂ŁA�A��ł���Ȃ��Ȃ邩��
    DirectX::XMFLOAT4 materialColor = { 1,1,1,1 };
    void SetMaterialColor(const DirectX::XMFLOAT4& color) { materialColor = color; }

    // �g�����X�t�H�[���擾
    Transform* GetTransform() const  { return model->GetTransform(); }

    DirectX::XMFLOAT4X4 SetDebugModelTransform(DirectX::XMFLOAT4X4 transform);

public: // �A�j���[�V���������֐��֘A�i�V���[�g�J�b�g�֐��j

    // �A�j���[�V�����Đ��ݒ�
    // (�A�j���[�V�����ԍ��E���[�v���邩�ǂ����E�A�j���[�V�����Đ����x�E�X���[�Y�؂�ւ����ԁi���x�j)
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
    void UpdateVelocity(const float& elapsedTime);          // ���͏����X�V����
    void UpdateAABB(const float& elapsedTime);              // AABB�X�V����
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

    void UpdateHorizontalVelocity(const float& elapsedFrame);   // �������͍X�V����
    void UpdateHorizontalMove(const float& elapsedTime);        // �����ړ��X�V����

public:
    std::unique_ptr<Model>  model       = nullptr;
    std::unique_ptr<Model>  debugModel  = nullptr;

    Collision::AABB aabb = {};  
    std::unique_ptr<GeometricPrimitive> geometricAABB_ = nullptr;

protected:
    DirectX::XMFLOAT3 velocity  =   { 0,0,0 };      // ���x
                                  
    float       stepOffset      =   1.0f;           // �ʒu�␳(Y�ʒu���L�����N�^�[�̒��S�ɂȂ�悤�ɒ���)
                                    
    float       moveVecX        =   0.0f;           // �ړ��x�N�g��X
    float       saveMoveVecX    =   1.0f;           // �ړ��x�N�g����ۑ�����x�N�g��X�i�ŏ��͉E���������Ă����j

    float       defaultAcceleration =   1.0f;                   // �����͂̏����l
    float       acceleration        =   defaultAcceleration;    // ������
    float       defaultGravity      =  -1.0f;                   // �d�͏����l
    float       gravity             =   defaultGravity;         // �d��
    float       defaultFriction     =   0.5f;                   // ���C�͂̏����l
    float       friction            =   0.5f;                   // ���C��
    float       airControl          =   0.3f;                   // �󒆐���

    float       defaultMoveSpeed =  5.0f;               // �ړ����x�����l
    float       moveSpeed        =  defaultMoveSpeed;   // �ړ����x(�ő�ړ����x�ɑ�������)
    float       turnSpeed        =  ToRadian(900.0f);   // ���񑬓x(180.f * 5)
    float       jumpSpeed        =  10.0f;              // �W�����v���x
    float       maxMoveSpeed     =  5.0f;               // �ő�ړ����x

    float       slopeRate       =   1.0f;           // �X�Η�
                                    
    float       invincibleTimer =   1.0f;           // ���G����

    float       defaultJumpTime =   0.3f;           // �W�����v����
    float       jumpTimer       =   0.0f;           // �W�����v�^�C�}�[

    int         jumpCount       =   0;              // �W�����v��
    int         jumpLimit       =   1;              // �ő�W�����v��
                                   
    bool        isGround        =   false;          // �n�ʂɂ��Ă��邩
    bool        isBounce        =   false;          // �o�E���X�����邩
    bool        isDash          =   false;          // �_�b�V�����Ă��邩


};

