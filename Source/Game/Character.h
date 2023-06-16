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
    Character() {}
    virtual ~Character() {}

    virtual void DrawDebug();   // �f�o�b�O�`��

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
    void UpdateInvincibleTimer(const float& elapsedTime);   // ���G���ԍX�V����

protected:
    virtual void OnLanding() {} // ���n�����Ƃ��ɌĂ΂��   
    virtual void OnBounce()  {} // �o�E���X����Ƃ��ɌĂ΂��
    virtual void OnDamaged() {} // �_���[�W���󂯂��Ƃ��ɌĂ΂��  
    virtual void OnDead()    {} // ���S�����Ƃ��ɌĂ΂��

private: 
    void UpdateVerticalVelocity(const float& elapsedFrame);     // �������͍X�V����
    void UpdateVerticalMove(const float& elapsedTime);          // �����ړ��X�V����

    void UpdateHorizontalVelocity(const float& elapsedFrame);   // �������͍X�V����
    void UpdateHorizontalMove(const float& elapsedTime);        // �����ړ��X�V����

public:
    static constexpr float BOUNCE_SPEED_X       =  10.0f;   // �o�E���XX���x
    static constexpr float BOUNCE_SPEED_Y       =  20.0f;   // �o�E���XY���x
    static constexpr float BOUNCE_SPEED_DIVIDE  =  0.75f;   // �o�E���X���x������l
    static constexpr float GRAVITY              = -1.0f;    // �d��
    static constexpr float BOUNCE_GRAVITY       = -3.0f;    // �o�E���h���̏d��
public:
    std::unique_ptr<Model>  model = nullptr;
    std::unique_ptr<Model> debugModel = nullptr;

    // AABB                    aabb  = {};
    Box2D box2d{};
    

    std::unique_ptr<GeometricPrimitive> geometricPrimitive;

protected:
    DirectX::XMFLOAT3 velocity      =   { 0,0,0 }; // ���x
    DirectX::XMFLOAT3 bounceSpeed   =   { BOUNCE_SPEED_X, BOUNCE_SPEED_Y, 0.0f };  // �o�E���X���x
                                  
    float       stepOffset      =   1.0f;        // �ʒu�␳(Y�ʒu���L�����N�^�[�̒��S�ɂȂ�悤�ɒ���)
                                    
    float       moveVecX        =   0.0f;        // �ړ��x�N�g��X
    float       saveMoveVecX    =   0.0f;        // �ړ��x�N�g����ۑ�����x�N�g��X

    float       acceleration    =   1.0f;        // ������
    float       gravity         =   GRAVITY;        // �d��
    float       friction        =   0.5f;        // ���C��
    float       airControl      =   0.3f;        // �󒆐���

    float       moveSpeed       =   5.0f;               // �ړ����x(�ő�ړ����x�ɑ�������)
    float       turnSpeed       =  ToRadian(900.0f);    // ���񑬓x
    //float       turnSpeed       =   ToRadian(720.0f);   // ���񑬓x
    float       jumpSpeed       =   20.0f;              // �W�����v���x
    float       maxMoveSpeed    =   5.0f;               // �ő�ړ����x

    float       slopeRate       =   1.0f;        // �X�Η�
                                    
    float       invincibleTimer =   1.0f;        // ���G����

    int         jumpCount       =   0;            // �W�����v��
    int         jumpLimit       =   1;            // �ő�W�����v��

    int         bounceCount      =   0;            // �o�E���X��
    int         bounceLimit      =   3;            // �ő�o�E���X��
                                   
    bool        isGround        =   false;        // �n�ʂɂ��Ă��邩
    bool        isBounce        =   false;        // �o�E���X�����邩
};

