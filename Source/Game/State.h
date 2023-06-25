#pragma once

class Enemy;

class State
{
public:
    State(Enemy* _boss, const char* name) :owner(_boss), name(name) {}
    virtual ~State() {}
    // すべての継承先で実装させる必要があるため純粋仮想関数で実装
    // ステートに入った時のメソッド
    virtual void Enter() = 0;
    // ステートで実行するメソッド
    virtual void Execute(float elapsedTime) = 0;
    // ステートから出ていくときのメソッド
    virtual void Exit() = 0;
    
public:
    const char* GetName() { return name; }
    void SetTimer(float time) { timer = time; }
    float GetTimer() const { return timer; }
    void SubtractTime(float time) { timer -= time; }


    void SetMoveSpeed(float speed) { moveSpeed = speed; }
    float GetMoveSpeed() { return moveSpeed; }
    void SetRotationSpeed(float rotSpeed) { rotationSpeed = rotSpeed; }
    float GetRotationSpeed() { return rotationSpeed; }

private:
    float timer = 0;
    
    float moveSpeed = 0.0f;     // 移動速度
    float rotationSpeed = 0.0f; // 回転速度

protected:
    Enemy* owner;
    const char* name;
};

