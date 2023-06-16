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
    // 移動処理
    void Move(
        const float& vx, 
        const float& vz, 
        const float& moveSpeed
    );

    // 旋回処理
    void Turn(
        const float& elapsedTime, 
        NO_CONST float vx, 
        NO_CONST float vz, 
        NO_CONST float turnSpeed
    );

    // ジャンプ処理
    void Jump(const float& jumpSpeed);

protected:
    // 速力処理更新処理
    void UpdateVelocity(const float& elapsedTime);

    // 無敵時間更新処理
    void UpdateInvincibleTimer(const float& elapsedTime);

protected:
    // 着地したときに呼ばれる
    virtual void OnLanding() {}

    // ダメージを受けた時に呼ばれる
    virtual void OnDamaged() {}

    // 死亡したときに呼ばれる
    virtual void OnDead() {}

private:
    // 垂直速力更新処理
    void UpdateVerticalVelocity(const float& elapsedFrame);
    // 垂直移動更新処理
    void UpdateVerticalMove(const float& elapsedTime);

    // 水平速力更新処理
    void UpdateHorizontalVelocity(const float& elapsedFrame);
    // 水平移動更新処理
    void UpdateHorizontalMove(const float& elapsedTime);

public:
    std::unique_ptr<Model> model = nullptr;
    std::unique_ptr<Model> debugModel = nullptr;
    AABB aabb{};

protected:


    DirectX::XMFLOAT3 velocity  =  { 0,0,0 };   // 速度
                                   
    float       stepOffset      =  1.0f;        // 位置補正(Y位置がキャラクターの中心になるように調整)
                                   
    float       moveVecX        =  0.0f;        // 移動ベクトルX
    float       moveVecZ        =  0.0f;        // 移動ベクトルY

    float       acceleration    =  1.0f;        // 加速力
    //float       gravity         = -1.0f;        // 重力
    float       gravity         =  0.0f;        // 重力
    float       friction        =  0.5f;        // 摩擦力
    float       airControl      =  0.3f;        // 空中制御

    float       moveSpeed       =  5.0f;                // 移動速度(最大移動速度に代入される)
    float       turnSpeed       =  ToRadian(720.0f);    // 旋回速度
    float       jumpSpeed       =  20.0f;               // ジャンプ速度
    float       maxMoveSpeed    =  5.0f;                // 最大移動速度

    float       slopeRate       =  1.0f;        // 傾斜率
                                   
    float       invincibleTimer =  1.0f;        // 無敵時間

    int         jumpCount       = 0;            // ジャンプ回数
    int         jumpLimit       = 2;            // 最大ジャンプ回数
                                   
    bool        isGround        =  false;       // 地面についているか
};

