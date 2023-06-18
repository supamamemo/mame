#include "Boss.h"
#include "BossStateDerived.h"

#include "PlayerManager.h"

// IdleState
namespace BOSS
{
    // 初期化
    void IdleState::Enter()
    {
        // アニメーションセット
        owner->SetAnimation(static_cast<int>(BossAnimation::Idle));

        // materialColorを設定(セーフティー(青))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 1));

        // タイマーをセット
        SetTimer(2.0f);
    }

    // 更新
    void IdleState::Execute(float elapsedTime)
    {
        // タイマーが0になったらFindeステートへ
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));

        // 時間を減らす
        SubtractTime(elapsedTime);

    }

    // 終了
    void IdleState::Exit()
    {

    }
}

// FindState
namespace BOSS
{
    // 初期化
    void FindState::Enter()
    {
        // アニメーションセット
        owner->SetAnimation(static_cast<int>(BossAnimation::Find));

        // materialColor(見つけた(黄色))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));

        // タイマーをセット
        SetTimer(2.0f);
    }

    // 更新
    void FindState::Execute(float elapsedTime)
    {
        // タイマーが0になったらAttackStateへ
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));

        // 時間を減らす
        SubtractTime(elapsedTime);
    }

    // 終了
    void FindState::Exit()
    {
    }
}

// AttackState
namespace BOSS
{
    // 初期化
    void AttackState::Enter()
    {
        // アニメーションセット
        owner->SetAnimation(static_cast<int>(BossAnimation::Attack));

        // materialColorを設定(アグレッシブ(赤))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 1));

        // position取得
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->model->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        // 左右判定
        moveSpeed = playerPos.x > ownerPos.x ? 1 : -1;
    }

    // 更新
    void AttackState::Execute(float elapsedTime)
    {
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        ownerPos.x += moveSpeed * elapsedTime;

        // 壁にぶつかったらIdleステートへ
        if (ownerPos.x > 3.3f || ownerPos.x < -4.0f)
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));

        //// タイマーが0になったらIdleステートへ
        //if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));

        //// 時間を減らす
        //SubtractTime(elapsedTime);

        //DirectX::XMFLOAT3 pos = owner->GetTransform()->GetPosition();
        //float moveSpeed = moveLeft ? -1 : 1;
        //pos.x += moveSpeed * elapsedTime;
        //owner->GetTransform()->SetPosition(pos);

        //if (pos.x > 3)moveLeft = true;
        //if (pos.x < -3)moveLeft = false;

    }

    // 終了
    void AttackState::Exit()
    {

    }
}