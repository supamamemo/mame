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
    bool FindPlayer();  // �v���C���[�����G����(�����̕��s�ƒǐՃX�e�[�g�Ŏg�����������̂ł����Ɉړ�)
    
public:
    const char* GetName() { return name; }
    void SetTimer(float time) { timer = time; }
    float GetTimer() const { return timer; }
    void SubtractTime(float time) { timer -= time; }
public:
    float seTimer_ = 0.0f;

private:
    float timer = 0;
    
    float rotationSpeed = 0.0f; // ��]���x


protected:
    Enemy* owner;
    const char* name;
};

