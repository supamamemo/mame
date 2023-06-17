#pragma once
#include "State.h"

// �Ƃ肠���������ɒu�����ǁABOSS���p��������ɂ��ꂼ��ɍ��B
// BOSSTOUHU::STATE::Idle (��)
enum class STATE
{
    Idle,
    Attack,
};

namespace BOSS
{
    class IdleState :public State
    {
    public:
        IdleState(Boss* _boss) :State(_boss, "idle") {}
        ~IdleState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class AttackState :public State
    {
    public:
        AttackState(Boss* _boss) :State(_boss, "attack") {}
        ~AttackState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };
}

