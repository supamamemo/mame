#pragma once
#include <vector>
#include "State.h"

// ステートマシーン
class StateMachine
{
public:
    StateMachine() {};
    ~StateMachine();

    // 更新処理
    void Update(float elapsedTime);

    // ステートセット
    void SetState(int newState);

    // ステート変更
    void ChangeState(int newState);

    // ステート登録
    void RegisterState(State* state);

    // 現在のステート番号取得
    int GetStateIndex();

private:
    // 現在のステート
    State* currentState = nullptr;
    // 各ステートを保持する配列
    std::vector<State*> statePool;

};

