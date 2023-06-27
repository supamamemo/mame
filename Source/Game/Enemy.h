#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "CannonBallManager.h"


class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize()               = 0; // 初期化
    virtual void Finalize()                 = 0; // 終了化
    virtual void Begin()                    = 0; // 毎フレーム一番最初に呼ばれる
    virtual void Update(float elapsedTime)  = 0; // 更新処理
    virtual void End()                      = 0; // 毎フレーム一番最後に呼ばれる
    virtual void Render(float elapsedTime)  = 0; // 描画処理
    virtual void DrawDebug()                = 0; // デバッグ描画

public:
    // ステートマシン
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

public:    
    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // ステートマシン

    CannonBallManager cannonBallManager = {};               // 大砲の弾マネージャー

public:
    void SetMoveRight(bool r) { moveRight = r; }
    bool GetMoveRight() { return moveRight; }
private:
    bool moveRight = true;      // 右に進むかどうか
};

