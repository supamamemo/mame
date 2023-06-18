#pragma once
#include "State.h"

// とりあえずここに置くけど、BOSSを継承した先にそれぞれに作る。
// BOSSTOUHU::STATE::Idle (例)
enum class STATE
{
    Idle,
    Find,
    Attack,
    Recoil,
};

namespace BOSS
{
    class IdleState : public State
    {
    public:
        IdleState(Boss* _boss) :State(_boss, "idle") {}
        ~IdleState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class FindState : public State
    {
    public:
        FindState(Boss* _boss) :State(_boss, "find") {}
        ~FindState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class AttackState : public State
    {
    public:
        AttackState(Boss* _boss) :State(_boss, "attack") {}
        ~AttackState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;

    private:
        float moveSpeed = 0;

        bool moveLeft = true;
    };
}

