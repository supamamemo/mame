#pragma once
#include "State.h"

// とりあえずここに置くけど、BOSSを継承した先にそれぞれに作る。
// BOSSTOUHU::STATE::Idle (例)
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

    private:
        bool moveLeft = true;
    };
}

