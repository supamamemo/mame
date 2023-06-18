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

private: // 瞬間的に呼ばれる関数関連
    void OnLanding() override;  // 着地したときに呼ばれる   
    void OnBounce()  override;  // バウンスするときに呼ばれる
    void OnDamaged() override;  // ダメージを受けたときに呼ばれる
    void OnDead()    override;  // 死亡したときに呼ばれる

private: // ステート関数関連
    void TransitionIdleState();                     // 待機ステートへ遷移
    void UpdateIdleState(const float& elapsedTime); // 待機ステート更新処理
     
    void TransitionMoveState();                     // 移動ステートへ遷移   
    void UpdateMoveState(const float& elapsedTime); // 移動ステート更新処理
 
    void TransitionJumpState();                     // ジャンプステートへ遷移   
    void UpdateJumpState(const float& elapsedTime); // ジャンプステート更新処理    
    
    void TransitionHipDropState();                     // ヒップドロップステートへ遷移   
    void UpdateHipDropState(const float& elapsedTime); // ヒップドロップステート更新処理

private: // enum関連
    // ステート
    enum class State
    {
        Idle,    // 待機
        Move,    // 移動
        Jump,    // ジャンプ
        HipDrop, // ヒップドロップ
    };

private: // 初期値関連

private: // 変数関連
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders = {};

<<<<<<< HEAD


    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders;
=======
    DirectX::XMFLOAT3 translation   = { 0, 0, 0 };
    DirectX::XMFLOAT3 scaling       = { 1, 1, 1 };
    DirectX::XMFLOAT3 rotation      = { 0, 0, 0 };

    State   state                   = State::Idle;          // 現在のステート

    float   hipDropGravity          = -3.0f;                // ヒップドロップ時の重力

    float   defaultBounceSpeedX     =  15.0f;               // バウンスX速度初期値
    float   defaultBounceSpeedY     =  10.0f;               // バウンスY速度初期値
    float   bounceSpeedX            =  defaultBounceSpeedX; // バウンスX速度
    float   bounceSpeedY            =  defaultBounceSpeedY; // バウンスY速度
    float   bounceScaleX            =  0.75f;               // バウンスX速度にかける値
    float   bounceScaleY            =  0.75f;               // バウンスY速度にかける値

    float   saveMoveVec_n           =  0.0f;                // プレイヤーの前方向の単位ベクトルを保存する（バウンス時に使われる）

    int     bounceCount             =  0;                   // バウンス回数
    int     bounceLimit             =  3;                   // 最大バウンス回数

    int     animationIndex          =  0;
>>>>>>> origin/Taki
};

