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

    virtual void Render(const float& elapsedTime);  // 描画処理
    virtual void DrawDebug();                       // デバッグ描画

public:
    // ダメージを与える
    virtual bool ApplyDamage(
        const int& damage, 
        const float& invincibleTime
    );

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
    void UpdateAABB();                                      // AABB更新処理
    void UpdateVelocity(const float& elapsedTime);          // 速力処理更新処理
    void UpdateInvincibleTimer(const float& elapsedTime);   // 無敵時間更新処理

protected:
    virtual void OnLanding() {}                             // 着地したときに呼ばれる  
    virtual void OnDash()    {}                             // ダッシュしているときに呼ばれる
    virtual void OnBounce()  {}                             // バウンスするときに呼ばれる
    virtual void OnDamaged() {}                             // ダメージを受けたときに呼ばれる  
    virtual void OnDead()    {}                             // 死亡したときに呼ばれる

private: 
    void UpdateVerticalVelocity(const float& elapsedFrame);     // 垂直速力更新処理
    void UpdateVerticalMove(const float& elapsedTime);          // 垂直移動更新処理

    void VerticalFall(const float& fallSpeed);                  // 垂直落下処理(コードが長くなったので落下時の処理をこちらに移動)
    void VerticalRise(const float& riseSpeed);                  // 垂直上昇処理(コードが長くなったので上昇時の処理をこちらに移動)

    void UpdateHorizontalVelocity(const float& elapsedFrame);   // 水平速力更新処理
    void UpdateHorizontalMove(const float& elapsedTime);        // 水平移動更新処理

    void HorizontalRightLeft(NO_CONST float horizontalSpeed);     // 水平移動処理(コードが長くなったので水平移動時の処理をこちらに移動)


public: // アニメーション処理関数関連（ショートカット関数）

    // アニメーション再生設定
    void PlayAnimation(
        const int& index,
        const bool& loop,
        const float& speed = 1.0f,
        const float& blendSeconds = 1.0f)
    {
        model->PlayAnimation(index, loop, speed, blendSeconds);
    }

    // アニメーション更新処理
    void UpdateAnimation(const float& elapsedTime) { model->UpdateAnimation(elapsedTime); }

    // アニメーションが再生中かどうか
    bool IsPlayAnimation() const { return model->IsPlayAnimation(); }

    // アニメーション再生速度設定（途中で再生速度を変えたいときなどに）
    void SetAnimationSpeed(const float& speed) { model->SetAnimationSpeed(speed); }

public: // 取得・設定関数関連

    // トランスフォーム取得
    Transform* GetTransform() const { return model->GetTransform(); }

    // カラー設定
    void SetMaterialColor(const DirectX::XMFLOAT4& color) { materialColor = color; }

    // 無敵かどうかの設定・取得
    const float& GetIsInvincible() const { return isInvincible; }
    void SetIsInvincible(const float& invincible) { isInvincible = invincible; }

    // AABB再設定(当たり判定のサイズを途中で変えたいときなどに)
    void ResetAABB(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);

public:
    std::unique_ptr<Model> model    =  nullptr;                     // モデル

    std::unique_ptr<GeometricPrimitive> geometricAABB_ = nullptr;   // 当たり判定描画用ジオメトリックプリミティブ
    Collision::AABB aabb_           = {};                           // 当たり判定構造体

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders;

    DirectX::XMFLOAT4 materialColor =   { 1.0f, 0.0f, 0.0f, 0.4f }; // マテリアルカラー

    DirectX::XMFLOAT3 velocity      =   { 0,0,0 };                  // 速度
           
    float       modelColorAlpha     =   1.0f;

    float       stepOffset          =   1.0f;                       // 位置補正(Y位置がキャラクターの中心になるように調整)
                                        
    float       moveVecX            =   0.0f;                       // 移動ベクトルX
    float       saveMoveVecX        =   1.0f;                       // 移動ベクトルを保存するベクトルX（最初は右を向かせておく）

    float       defaultAcceleration =   1.0f;                       // 加速力の初期値
    float       acceleration        =   defaultAcceleration;        // 加速力
    float       defaultGravity      =  -1.0f;                       // 重力初期値
    float       gravity             =   defaultGravity;             // 重力
    float       defaultFriction     =   0.5f;                       // 摩擦力の初期値
    float       friction            =   0.5f;                       // 摩擦力
    float       airControl          =   0.3f;                       // 空中制御

    float       defaultMoveSpeed    =  5.0f;                        // 移動速度初期値
    float       moveSpeed           =  defaultMoveSpeed;            // 移動速度(最大移動速度に代入される)
    float       turnSpeed           =  ToRadian(900.0f);            // 旋回速度(180.f * 5)
    float       jumpSpeed           =  10.0f;                       // ジャンプ速度
    float       maxMoveSpeed        =  5.0f;                        // 最大移動速度

    float       slopeRate           =   1.0f;                       // 傾斜率
                                        
    float       invincibleTimer     =   0.0f;                       // 無敵時間

    float       defaultJumpTime     =   0.3f;                       // ジャンプ時間
    float       jumpTimer           =   0.0f;                       // ジャンプタイマー

    int         health              =   3;                          // 体力
    int         jumpCount           =   0;                          // ジャンプ回数
    int         jumpLimit           =   1;                          // 最大ジャンプ回数
                                       
    bool        isGround            =   false;                      // 地面についているか
    bool        isBounce            =   false;                      // バウンスさせるか
    bool        isDash              =   false;                      // ダッシュしているか
    bool        isInvincible        =   false;                      // 無敵かどうか(ボスに使う)

private:
    // 大きさの変更をするときに当たり判定のAABB描画の大きさも変更させるためにprivateにしている
    DirectX::XMFLOAT3 defaultMin_   = { -0.4f, -0.0f, -0.4f };       // 当たり判定のminデフォルト値
    DirectX::XMFLOAT3 defaultMax_   = { +0.4f, +0.8f, +0.4f };       // 当たり判定のmaxデフォルト値
};

