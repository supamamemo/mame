#pragma once
#include "State.h"

// Ç∆ÇËÇ†Ç¶Ç∏Ç±Ç±Ç…íuÇ≠ÇØÇ«ÅABOSSÇåpè≥ÇµÇΩêÊÇ…ÇªÇÍÇºÇÍÇ…çÏÇÈÅB
// BOSSTOUHU::STATE::Idle (ó·)
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

