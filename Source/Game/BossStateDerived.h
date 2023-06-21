#pragma once
#include "State.h"

// Ç∆ÇËÇ†Ç¶Ç∏Ç±Ç±Ç…íuÇ≠ÇØÇ«ÅABOSSÇåpè≥ÇµÇΩêÊÇ…ÇªÇÍÇºÇÍÇ…çÏÇÈÅB
// BOSSTOUHU::STATE::Idle (ó·)

namespace BOSS
{
    enum class STATE
    {
        Idle,
        Find,
        Attack,
        Recoil,
    };
}

namespace CANNON
{
    enum class STATE
    {
        Idle,
        Attack,
    };
}

namespace BOSS
{
    class IdleState : public State
    {
    public:
        IdleState(Enemy* _boss) :State(_boss, "idle") {}
        ~IdleState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class FindState : public State
    {
    public:
        FindState(Enemy* _boss) :State(_boss, "find") {}
        ~FindState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class AttackState : public State
    {
    public:
        AttackState(Enemy* _boss) :State(_boss, "attack") {}
        ~AttackState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;

    private:
        float moveSpeed = 0;

        bool moveLeft = true;
    };

    class RecoilState : public State
    {
    public:
        RecoilState(Enemy* _boss) :State(_boss, "recoil") {}
        ~RecoilState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;

    public:
        void SetRecoil(float r) { recoil = r; }

    private:
        float recoil = 0;       // îΩìÆãóó£
        float recoilCount = 0;  // ç°Ç«ÇÍÇæÇØîΩìÆÇ≈îÚÇÒÇ≈Ç¢ÇÈÇ©
    };
}

namespace TOFU
{
    
}

namespace CANNON
{
    class IdleState : public State
    {
        IdleState(Enemy* _cannon) : State(_cannon, "idle") {}
        ~IdleState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class AttackState : public State
    {
        AttackState(Enemy* _cannon) :State(_cannon, "attack") {}
        ~AttackState() {};

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };
}