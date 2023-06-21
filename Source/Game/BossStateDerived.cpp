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
        owner->PlayAnimation(static_cast<int>(BossAnimation::Idle), true);
                
        // materialColorを設定(セーフティー(青))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 1));

        // タイマーをセット
        SetTimer(2.0f);
    }

    // 更新
    void IdleState::Execute(float elapsedTime)
    {
        // アニメーション更新
        owner->UpdateAnimation(elapsedTime);

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
        owner->PlayAnimation(static_cast<int>(BossAnimation::Find), true);

        // materialColor(見つけた(黄色))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));

        // タイマーをセット
        SetTimer(2.0f);
    }

    // 更新
    void FindState::Execute(float elapsedTime)
    {
        // アニメーション更新
        owner->UpdateAnimation(elapsedTime);

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
        owner->PlayAnimation(static_cast<int>(BossAnimation::Attack), true);

        // materialColorを設定(アグレッシブ(赤))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 1));

        // position取得
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        // 左右判定
        moveSpeed = playerPos.x > ownerPos.x ? 3.0f : -3.0f;
    }

    // 更新
    void AttackState::Execute(float elapsedTime)
    {
        // アニメーション更新
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        ownerPos.x += moveSpeed * elapsedTime;

        // 壁にぶつかったらIdleステートへ
        if (ownerPos.x > 3.3f)
        {
            ownerPos.x = 3.3f;
            owner->GetStateMachine()->SetMoveRight(false);
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Recoil));
        }
        if (ownerPos.x < -4.0f)
        {
            ownerPos.x = -4.0f;
            owner->GetStateMachine()->SetMoveRight(true);
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Recoil));
        }

        owner->GetTransform()->SetPosition(ownerPos);
    }

    // 終了
    void AttackState::Exit()
    {

    }
}

// RecoilState
namespace BOSS
{
    // 初期化
    void RecoilState::Enter()
    {
        // アニメーションセット
        owner->PlayAnimation(static_cast<int>(BossAnimation::Recoil), true);

        // materialColorを設定(紫)
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
        
        // 反動距離を設定
        recoilCount = 0;
        
        SetRecoil(owner->GetStateMachine()->GetMoveRight() ? 1.0f : -1.0f);

    }

    // 更新
    void RecoilState::Execute(float elapsedTime)
    {
        // アニメーション更新
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        float moveSpeed = owner->GetStateMachine()->GetMoveRight() ? 2.0f : -2.0f;
        ownerPos.x += moveSpeed * elapsedTime;
        recoilCount += moveSpeed * elapsedTime;

        if (owner->GetStateMachine()->GetMoveRight())
        {
            if (recoilCount > recoil)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));
        }
        else
        {
            if (recoilCount < recoil)owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));
        }
        
        owner->GetTransform()->SetPosition(ownerPos);
    }

    // 終了
    void RecoilState::Exit()
    {
        recoilCount = 0;
    }
}