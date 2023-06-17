#pragma once

class Boss;

class State
{
public:
    State(Boss* _boss, const char* name) :owner(_boss), name(name) {}
    virtual ~State() {}
    // ���ׂĂ̌p����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
    // �X�e�[�g�ɓ��������̃��\�b�h
    virtual void Enter() = 0;
    // �X�e�[�g�Ŏ��s���郁�\�b�h
    virtual void Execute(float elapsedTime) = 0;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    virtual void Exit() = 0;
    
public:
    void SetTimer(float time) { timer = time; }
    float GetTimer() const { return timer; }
    void SubtractTime(float time) { timer -= time; }

private:
    float timer = 0;

protected:
    Boss* owner;
    const char* name;
};

