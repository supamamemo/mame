#pragma once
#include "State.h"


// �Ƃ肠���������ɒu�����ǁABOSS���p��������ɂ��ꂼ��ɍ��B
// BOSSTOUHU::STATE::Idle (��)

namespace BOSS
{
    enum class STATE
    {
        Idle,
        Find,
        Rotate,
        Attack,
        Recoil,
    };
}

namespace TOFU
{
    enum class STATE
    {
        Walk,
        Turn,
        Find,
        Track,
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

    class RotateState : public State
    {
    public:
        RotateState(Enemy* _boss) :State(_boss, "rotate") {}
        ~RotateState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;

    private:
        float rotate = 0.0f;
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
        bool moveLeft = true;
        float speed = 5.0f;
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
        float recoil = 0;       // ��������
        float recoilCount = 0;  // ���ǂꂾ�������Ŕ��ł��邩
        float speed = 2.0f;
    };
}

namespace TOFU
{
    class WalkState : public State
    {
    public:
        WalkState(Enemy* _tofu) :State(_tofu, "walk") {}
        ~WalkState() {}

        void Enter()override;
        void Execute(float elapsedTime);
        void Exit()override;

        void FindPlayer();

    private:
        float destination = 0.0f;   // �ړI�n
    };

    class TurnState :public State
    {
    public:
        TurnState(Enemy* _tofu) :State(_tofu, "turn") {}
        ~TurnState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;

    private:
        float rotate = 0.0f;
    };

    class FindState :public State
    {
    public:
        FindState(Enemy* _tofu) :State(_tofu, "find") {}
        ~FindState() {}


        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;

    private:
        float returnPositionY = 0.0f; // ���X�����ʒuY
        int state = 0;
    };

    class TrackState :public State
    {
    public:
        TrackState(Enemy* _tofu) :State(_tofu, "track") {}
        ~TrackState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };
}

namespace CANNON
{
    class IdleState : public State
    {
    public:
        IdleState(Enemy* _cannon) : State(_cannon, "idle") {}
        ~IdleState() {}

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class AttackState : public State
    {
    public:
        AttackState(Enemy* _cannon) :State(_cannon, "attack") {}
        ~AttackState() {};

        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };
}