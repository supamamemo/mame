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
    void Update(const float& elapsedTime);                          // 更新処理
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

private: // 変数関連
    State   state           = State::Idle; // 現在のステート



    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders;
};

