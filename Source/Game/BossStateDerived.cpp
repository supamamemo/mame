#include "BossStateDerived.h"

// 初期化
void BOSS::IdleState::Enter()
{
    // アニメーションセット
    owner->SetAnimation(static_cast<int>(BossAnimation::Idle));
}

// 更新
void BOSS::IdleState::Execute(float elapsedTime)
{

}

// 終了
void BOSS::IdleState::Exit()
{

}

// 初期化
void BOSS::AttackState::Enter()
{
    // アニメーションセット
    owner->SetAnimation(static_cast<int>(BossAnimation::Attack));
}

// 更新
void BOSS::AttackState::Execute(float elapsedTime)
{

}

// 終了
void BOSS::AttackState::Exit()
{

}
