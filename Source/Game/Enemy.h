#pragma once
#include "Character.h"
#include "StateMachine.h"

class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize() = 0;              // 初期化
    virtual void Finalize() = 0;                // 終了化
    virtual void Begin() = 0;                   // 毎フレーム一番最初に呼ばれる
    virtual void Update(float elapsedTime) = 0; // 更新処理
    virtual void End() = 0;                     // 毎フレーム一番最後に呼ばれる
    virtual void Render(float elapsedTime) = 0; // 描画処理
    virtual void DrawDebug() = 0;

public:
    // アニメーション再生設定
    void PlayAnimation(const int& index, const bool& loop);

    // アニメーション更新処理
    void UpdateAnimation(const float& elapsedTime);

    // アニメーションが再生中かどうか
    bool IsPlayAnimation() const;

    // ステートマシン
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

    
    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // ステートマシン


};

