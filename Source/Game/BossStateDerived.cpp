#include "Boss.h"
#include "BossStateDerived.h"


// 初期化
void BOSS::IdleState::Enter()
{
    // アニメーションセット
    owner->SetAnimation(static_cast<int>(BossAnimation::Idle));

    // materialColorを設定(セーフティー(青))
    owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 1));

    // タイマーをセット
    SetTimer(2.0f);
}

// 更新
void BOSS::IdleState::Execute(float elapsedTime)
{
    // 時間を減らす
    SubtractTime(elapsedTime);

    // タイマーが0になったらAttackステートへ
    if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
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

    // materialColorを設定(アグレッシブ(赤))
    owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 1));

    // タイマーをセット
    SetTimer(3.0f);
}

// 更新
void BOSS::AttackState::Execute(float elapsedTime)
{
    // 時間を減らす
    SubtractTime(elapsedTime);

    // タイマーが0になったらIdleステートへ
    if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));
}

// 終了
void BOSS::AttackState::Exit()
{

}
