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

    virtual void DrawDebug();   // デバッグ描画

protected:
    // 移動処理
    void Move(const float& vx, const float& moveSpeed);

    // 旋回処理
    void Turn(
        const float& elapsedTime, 
        NO_CONST float vx, 
        NO_CONST float turnSpeed
    );

    // ジャンプ処理
    void Jump(const float& jumpSpeed);

protected:
    void UpdateVelocity(const float& elapsedTime);          // 速力処理更新処理
    void UpdateInvincibleTimer(const float& elapsedTime);   // 無敵時間更新処理

protected:
    virtual void OnLanding() {} // 着地したときに呼ばれる   
    virtual void OnBounce()  {} // バウンスするときに呼ばれる
    virtual void OnDamaged() {} // ダメージを受けたときに呼ばれる  
    virtual void OnDead()    {} // 死亡したときに呼ばれる

private: 
    void UpdateVerticalVelocity(const float& elapsedFrame);     // 垂直速力更新処理
    void UpdateVerticalMove(const float& elapsedTime);          // 垂直移動更新処理

    void UpdateHorizontalVelocity(const float& elapsedFrame);   // 水平速力更新処理
    void UpdateHorizontalMove(const float& elapsedTime);        // 水平移動更新処理

public:
    static constexpr float BOUNCE_SPEED_X       =  10.0f;   // バウンスX速度
    static constexpr float BOUNCE_SPEED_Y       =  20.0f;   // バウンスY速度
    static constexpr float BOUNCE_SPEED_DIVIDE  =  0.75f;   // バウンス速度を割る値
    static constexpr float GRAVITY              = -1.0f;    // 重力
    static constexpr float BOUNCE_GRAVITY       = -3.0f;    // バウンド時の重力
public:
    std::unique_ptr<Model>  model = nullptr;
    std::unique_ptr<Model> debugModel = nullptr;

    // AABB                    aabb  = {};
    Box2D box2d{};
    

    std::unique_ptr<GeometricPrimitive> geometricPrimitive;

protected:
    DirectX::XMFLOAT3 velocity      =   { 0,0,0 }; // 速度
    DirectX::XMFLOAT3 bounceSpeed   =   { BOUNCE_SPEED_X, BOUNCE_SPEED_Y, 0.0f };  // バウンス速度
                                  
    float       stepOffset      =   1.0f;        // 位置補正(Y位置がキャラクターの中心になるように調整)
                                    
    float       moveVecX        =   0.0f;        // 移動ベクトルX
    float       saveMoveVecX    =   0.0f;        // 移動ベクトルを保存するベクトルX

    float       acceleration    =   1.0f;        // 加速力
    float       gravity         =   GRAVITY;        // 重力
    float       friction        =   0.5f;        // 摩擦力
    float       airControl      =   0.3f;        // 空中制御

    float       moveSpeed       =   5.0f;               // 移動速度(最大移動速度に代入される)
    float       turnSpeed       =  ToRadian(900.0f);    // 旋回速度
    //float       turnSpeed       =   ToRadian(720.0f);   // 旋回速度
    float       jumpSpeed       =   20.0f;              // ジャンプ速度
    float       maxMoveSpeed    =   5.0f;               // 最大移動速度

    float       slopeRate       =   1.0f;        // 傾斜率
                                    
    float       invincibleTimer =   1.0f;        // 無敵時間

    int         jumpCount       =   0;            // ジャンプ回数
    int         jumpLimit       =   1;            // 最大ジャンプ回数

    int         bounceCount      =   0;            // バウンス回数
    int         bounceLimit      =   3;            // 最大バウンス回数
                                   
    bool        isGround        =   false;        // 地面についているか
    bool        isBounce        =   false;        // バウンスさせるか
};

