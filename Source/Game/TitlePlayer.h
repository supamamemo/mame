#pragma once

#include "Character.h"

// タイトル用プレイヤー
class TitlePlayer : public Character
{
public:
    TitlePlayer();
    ~TitlePlayer() override;

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Begin() {}                         // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);  // 更新処理
    void End()   {}                         // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime);  // 描画処理

    void DrawDebug() override;              // デバッグ描画

private:
    // 円運動
    void CircularMotion(const float elapsedTime);

    void Turn(
        const float elapsedTime, 
        NO_CONST float vx,
        NO_CONST float vz,
        NO_CONST float turnSpeed
    );
private:
    // アニメーション
    enum Animation
    {
        //Anim_Idle,      // 待機
        //Anim_Dash,      // ダッシュ（一時的な急加速）
        Anim_Run = 2,       // 走行
        //Anim_Break,     // ブレーキ
        //Anim_JumpInit,  // ジャンプ開始
        //Anim_Jump,      // ジャンプ
        //Anim_Fall,      // 落下
        //Anim_JumpEnd,   // 着地（ジャンプ終了）
        //Anim_HipDrop,   // ヒップドロップ
        //Anim_Walk,      // 歩行
        //Anim_Max,       // アニメーション最大数
    };

    // XMFLOATのメンバ変数がYとZで紛らわしいので作成
    struct Vector2D_XZ
    {
        float x = 0.0f;
        float z = 0.0f;
    };

    // 円
    struct Circle
    {
        Vector2D_XZ center     = { 0,0 };   // 円の中心
        float       radius     = 0.0f;      // 円の半径
        Vector2D_XZ unitCircle = { 0,0 };   // 単位円(単位ベクトル)
    };

private:
    Circle circle = { {}, 20.0f, {} };          // 円
    float rotationY      = 0.0f;                // 回転値(トランスフォームの角度を直接いじると移動速度も影響を受けて爆速になってしまうので別で用意している)
    float rotationSpeedY = ToRadian(-45.0f);    // 回転速度Y（移動速度）
};