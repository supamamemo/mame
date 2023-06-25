#pragma once

class Enemy;

class State
{
public:
    State(Enemy* _boss, const char* name) :owner(_boss), name(name) {}
    virtual ~State() {}
    // ���ׂĂ̌p����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
    // �X�e�[�g�ɓ��������̃��\�b�h
    virtual void Enter() = 0;
    // �X�e�[�g�Ŏ��s���郁�\�b�h
    virtual void Execute(float elapsedTime) = 0;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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
    
    float moveSpeed = 0.0f;     // �ړ����x
    float rotationSpeed = 0.0f; // ��]���x

protected:
    Enemy* owner;
    const char* name;
};

