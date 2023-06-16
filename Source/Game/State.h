#pragma once

#include "Boss.h"

class State
{
public:
    State(Boss* _boss, const char* name) :owner(_boss), name(name) {}
    virtual ~State() {}
    // すべての継承先で実装させる必要があるため純粋仮想関数で実装
    // ステートに入った時のメソッド
    virtual void Enter() = 0;
    // ステートで実行するメソッド
    virtual void Execute(float elapsedTime) = 0;
    // ステートから出ていくときのメソッド
    virtual void Exit() = 0;
    
protected:
    Boss* owner;
    const char* name;
};

