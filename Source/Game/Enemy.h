#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "CannonBallManager.h"


class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize()                       = 0;    // 初期化
    virtual void Finalize()                         = 0;    // 終了化
    virtual void Begin()                            = 0;    // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime)   = 0;    // 更新処理
    virtual void End()                              = 0;    // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float& elapsedTime);          // 描画処理
    virtual void DrawDebug()                        = 0;    // デバッグ描画

    void OnFallDead() override;                             // 落下死・落下ミスしたときに呼ばれる

    void Destroy();                                         // エネミー削除

    void CollisionEnemyVsPlayer();                          // エネミーとプレイヤーの衝突判定処理  

public:
    // 旋回処理
    void Turn(
        const    float elapsedTime,
        const    float vx,
        NO_CONST float turnSpeed
    );

public:
    // ステートマシン
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

    const bool GetMoveRight() const { return moveRight; }
    void SetMoveRight(const bool r) { moveRight = r; }

    // 移動範囲の半径の取得・設定
    const float& GetMoveRangeRadius() const { return moveRangeRadius_; }
    void SetMoveRangeRadius(const float& moveRange ) { moveRangeRadius_ = moveRange; }    
    
    // 移動範囲の中心Xの取得・設定
    const float& GetMoveRangeCenterX() const { return moveRangeCenterX_; }
    void SetMoveRangeCenterX(const float& moveRangeCenterX ) { moveRangeCenterX_ = moveRangeCenterX; }    
    
    // 索敵距離の取得・設定
    const float& GetSerchLength() const { return serchLength_; }
    void SetSerchLength(const float& serchLength) { serchLength_ = serchLength; }

    // 移動方向Xの取得・設定
    const float& GetMoveDirectionX() const { return moveDirectionX_; }
    void SetMoveDirectionX(const float& moveDirectionX) { moveDirectionX_ = moveDirectionX; }

public: // static変数
    static float renderLengthXLimit_;   // 敵を描画する距離制限

public:    
    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // ステートマシン

    CannonBallManager cannonBallManager = {};               // 大砲の弾マネージャー

protected:
    float moveRangeCenterX_ = 0.0f;     // 移動範囲の中心X
    float moveRangeRadius_  = 6.0f;     // 移動範囲の半径
    float serchLength_      = 6.0f;     // 索敵できる距離
    float moveDirectionX_   = 1.0f;     // 移動方向X  

private:
    bool  moveRight  = true;            // 右に進むかどうか
};

