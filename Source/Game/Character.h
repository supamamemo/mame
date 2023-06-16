#pragma once

#include "../Mame/Graphics/Model.h"

#ifdef USE_IMGUI
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_internal.h"
#include "../../External/imgui/imgui_impl_dx11.h"
#include "../../External/imgui/imgui_impl_win32.h"
#endif

#include "../GeometricPrimitive.h"

#include <DirectXMath.h>

#include "Common.h"

#include "Collision.h"

class Character
{
public:
    Character() {}
    virtual ~Character() {}

    virtual void DrawDebug();

protected:
    // �ړ�����
    void Move(
        const float& vx, 
        const float& vz, 
        const float& moveSpeed
    );

    // ���񏈗�
    void Turn(
        const float& elapsedTime, 
        NO_CONST float vx, 
        NO_CONST float vz, 
        NO_CONST float turnSpeed
    );

    // �W�����v����
    void Jump(const float& jumpSpeed);

protected:
    // ���͏����X�V����
    void UpdateVelocity(const float& elapsedTime);

    // ���G���ԍX�V����
    void UpdateInvincibleTimer(const float& elapsedTime);

protected:
    // ���n�����Ƃ��ɌĂ΂��
    virtual void OnLanding() {}

    // �_���[�W���󂯂����ɌĂ΂��
    virtual void OnDamaged() {}

    // ���S�����Ƃ��ɌĂ΂��
    virtual void OnDead() {}

private:
    // �������͍X�V����
    void UpdateVerticalVelocity(const float& elapsedFrame);
    // �����ړ��X�V����
    void UpdateVerticalMove(const float& elapsedTime);

    // �������͍X�V����
    void UpdateHorizontalVelocity(const float& elapsedFrame);
    // �����ړ��X�V����
    void UpdateHorizontalMove(const float& elapsedTime);

public:
    std::unique_ptr<Model> model = nullptr;
    std::unique_ptr<Model> debugModel = nullptr;
    AABB aabb{};

protected:


    DirectX::XMFLOAT3 velocity  =  { 0,0,0 };   // ���x
                                   
    float       stepOffset      =  1.0f;        // �ʒu�␳(Y�ʒu���L�����N�^�[�̒��S�ɂȂ�悤�ɒ���)
                                   
    float       moveVecX        =  0.0f;        // �ړ��x�N�g��X
    float       moveVecZ        =  0.0f;        // �ړ��x�N�g��Y

    float       acceleration    =  1.0f;        // ������
    //float       gravity         = -1.0f;        // �d��
    float       gravity         =  0.0f;        // �d��
    float       friction        =  0.5f;        // ���C��
    float       airControl      =  0.3f;        // �󒆐���

    float       moveSpeed       =  5.0f;                // �ړ����x(�ő�ړ����x�ɑ�������)
    float       turnSpeed       =  ToRadian(720.0f);    // ���񑬓x
    float       jumpSpeed       =  20.0f;               // �W�����v���x
    float       maxMoveSpeed    =  5.0f;                // �ő�ړ����x

    float       slopeRate       =  1.0f;        // �X�Η�
                                   
    float       invincibleTimer =  1.0f;        // ���G����

    int         jumpCount       = 0;            // �W�����v��
    int         jumpLimit       = 2;            // �ő�W�����v��
                                   
    bool        isGround        =  false;       // �n�ʂɂ��Ă��邩
};

