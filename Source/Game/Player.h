#pragma once

#include "Character.h"


class Player : public Character
{
public:
    Player();
    ~Player() override;

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Begin();                           // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);  // 更新処理
    void End();                             // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime);  // 描画処理

    void DrawDebug() override;              // デバッグ描画

private: // 入力処理関数関連   
    const float GetMoveVecX() const;                 // 左スティック入力値から移動X方向ベクトルを取得   
    const float GetMoveVecY() const;                 // 左スティック入力値から移動Y方向ベクトルを取得   
    const bool  InputMove(const float& elapsedTime); // 移動入力処理   
    const bool  InputJump();                         // ジャンプ入力処理

private: // 更新処理関数関連
    void  UpdateDashCoolTimer(const float& elapsedTime); // ダッシュクールタイム更新処理

private: // 瞬間的に呼ばれる関数関連
    void OnLanding() override;  // 着地したときに呼ばれる   
    void OnDash()    override;  // ダッシュしているときに呼ばれる
    void OnBounce()  override;  // バウンスするときに呼ばれる
    void OnDamaged() override;  // ダメージを受けたときに呼ばれる
    void OnDead()    override;  // 死亡したときに呼ばれる

private: // ステート関数関連
#if 1
    void TransitionIdleState();                         // 待機ステートへ遷移
    void UpdateIdleState(const float& elapsedTime);     // 待機ステート更新処理
     
    void TransitionWalkState();                         // 歩行ステートへ遷移   
    void UpdateWalkState(const float& elapsedTime);     // 歩行ステート更新処理    
    
    void TransitionDashState();                         // ダッシュステートへ遷移   
    void UpdateDashState(const float& elapsedTime);     // ダッシュステート更新処理    
    
    void TransitionRunState();                          // 走行ステートへ遷移   
    void UpdateRunState(const float& elapsedTime);      // 走行ステート更新処理
 
    void TransitionJumpState();                         // ジャンプステートへ遷移   
    void UpdateJumpState(const float& elapsedTime);     // ジャンプステート更新処理    
    
    void TransitionHipDropState();                      // ヒップドロップステートへ遷移   
    void UpdateHipDropState(const float& elapsedTime);  // ヒップドロップステート更新処理
#endif
private: // enum関連
    // ステート
    enum class State
    {
        Idle,    // 待機
        Walk,    // 歩行
        Dash,    // ダッシュ(一時的な急加速)
        Run,     // 走行
        Jump,    // ジャンプ
        HipDrop, // ヒップドロップ
    };

    // アニメーション
    enum Animation
    {
        Anim_Idle,      // 待機
        Anim_Dash,      // ダッシュ（一時的な急加速）
        Anim_Run,       // 走行
        Anim_Break,     // ?
        Anim_JumpInit,  // ジャンプ開始
        Anim_Jump,      // ジャンプ
        Anim_Fall,      // 落下
        Anim_JumpEnd,   // 着地（ジャンプ終了）
        Anim_HipDrop,   // ヒップドロップ
        Anim_Max,       // アニメーション最大数
    };

private: // 変数関連
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders = {};

    State   state                   = State::Idle;          // 現在のステート

    float   hipDropGravity          = -3.0f;                // ヒップドロップ時の重力

    float   dashAcceleration        =  30.0f;               // ダッシュ時のX速度
    float   defaultDashTime         =  0.1f;                // ダッシュ時間初期値
    float   dashTimer               =  defaultDashTime;     // ダッシュタイマー(ダッシュ時の操作制限時間を管理する)
    float   dashFinishScale         =  0.5f;                // ダッシュが終わって別のステート遷移前に速度を減速させる値
    float   dashCoolTime            =  0.5f;
    float   dashCoolTimer           =  dashCoolTime;        // ダッシュを連発されないようクールタイムを設定

    float   runMoveSpeed            =  15.0f;               // 走行時の移動速度(moveSpeedに代入する)

    float   defaultBounceSpeedX     =  15.0f;               // バウンスX速度初期値
    float   defaultBounceSpeedY     =  10.0f;               // バウンスY速度初期値
    float   bounceSpeedX            =  defaultBounceSpeedX; // バウンスX速度
    float   bounceSpeedY            =  defaultBounceSpeedY; // バウンスY速度
    float   bounceScaleX            =  0.75f;               // バウンスX速度にかける値
    float   bounceScaleY            =  0.75f;               // バウンスY速度にかける値
    float   saveMoveVec_n           =  0.0f;                // プレイヤーの単位移動ベクトルを保存する（バウンス時に使われる）

    int     bounceCount             =  0;                   // バウンス回数
    int     bounceLimit             =  2;                   // 最大バウンス回数

    bool    isUpdatedAnimation      = false;                //　アニメーションが更新されたか
};

