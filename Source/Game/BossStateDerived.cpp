#include "Boss.h"
#include "BossStateDerived.h"

#include "PlayerManager.h"

#include "CannonBall.h"

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
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Find));

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
        SetTimer(1.0f);
    }

    // 更新
    void FindState::Execute(float elapsedTime)
    {
        // アニメーション更新
        owner->UpdateAnimation(elapsedTime);

        // タイマーが0になったらAttackStateへ
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Rotate));

        // 時間を減らす
        SubtractTime(elapsedTime);
    }

    // 終了
    void FindState::Exit()
    {
    }
}

// RotateState
namespace BOSS
{
    // 初期化
    void RotateState::Enter()
    {
        // materialColor(回転(緑))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

        // 回転速度設定
        SetRotationSpeed(3.0f);

        // プレイヤーがどっちにいるのか
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        rotate = playerPos.x > ownerPos.x ? 90.0f : 270.0f;
    }

    // 更新
    void RotateState::Execute(float elapsedTime)
    {
        // プレイヤーの方向に向くように回転する
        {
            DirectX::XMFLOAT4 rotation = owner->GetTransform()->GetRotation();
            // 90度の場合
            if (rotate == 90.0f)
            {
                // 回転
                rotation.y -= GetRotationSpeed() * elapsedTime;
                
                // 回転終わったらAttackStateへ
                if (DirectX::XMConvertToRadians(rotate) >= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(90.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Attack));
                }
            }
            // -90度の場合
            else
            {
                // 回転
                rotation.y += GetRotationSpeed() * elapsedTime;

                // 回転終わったらAttackStateへ
                if (DirectX::XMConvertToRadians(rotate) <= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(270.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
                }
            }
            owner->GetTransform()->SetRotation(rotation);
        }
    }

    // 終了
    void RotateState::Exit()
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
        SetMoveSpeed(playerPos.x > ownerPos.x ? speed : -speed);
        float rot = playerPos.x > ownerPos.x ? 90.0f : -90.0f;
        owner->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(rot), 0.0f, 0.0f));
    }

    // 更新
    void AttackState::Execute(float elapsedTime)
    {
        // アニメーション更新
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        ownerPos.x += GetMoveSpeed() * elapsedTime;

        // 壁にぶつかったらIdleステートへ
        if (ownerPos.x > 9.0f)
        {
            ownerPos.x = 9.0f;
            owner->GetStateMachine()->SetMoveRight(false);
            owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Recoil));
        }
        if (ownerPos.x < -9.0f)
        {
            ownerPos.x = -9.0f;
            owner->GetStateMachine()->SetMoveRight(true);
            owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Recoil));
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
        SetMoveSpeed(owner->GetStateMachine()->GetMoveRight() ? speed : -speed);
        ownerPos.x += GetMoveSpeed() * elapsedTime;
        recoilCount += GetMoveSpeed() * elapsedTime;

        if (owner->GetStateMachine()->GetMoveRight())
        {
            if (recoilCount > recoil)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Idle));
        }
        else
        {
            if (recoilCount < recoil)owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Idle));
        }
        
        owner->GetTransform()->SetPosition(ownerPos);
    }

    // 終了
    void RecoilState::Exit()
    {
        recoilCount = 0;
    }
}

// WalkState
namespace TOFU
{
    // 初期化
    void WalkState::Enter()
    {
        // 移動速度設定
        SetMoveSpeed(1.0f);        

        // 目的地を設定
        destination = owner->GetMoveRight() ? owner->GetTransform()->GetPosition().x + 10.0f
            : owner->GetTransform()->GetPosition().x - 10.0f;
    }

    // 更新
    void WalkState::Execute(float elapsedTime)
    {
        // 移動処理
        {
            DirectX::XMFLOAT3 pos = owner->GetTransform()->GetPosition();
            if (owner->GetMoveRight())
            {
                pos.x += GetMoveSpeed() * elapsedTime;
                if (pos.x >= destination)owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Turn));
            }
            else
            {
                pos.x -= GetMoveSpeed() * elapsedTime;
                if (pos.x <= destination)owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Turn));
            }
            owner->GetTransform()->SetPosition(pos);
        }

        // playerとの範囲チェック
        FindPlayer();
    }

    // 終了
    void WalkState::Exit()
    {
    }

    // playerとの距離を計算して近くにいればFindStateへ
    void WalkState::FindPlayer()
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        // ownerからplayerへのベクトルを算出
        DirectX::XMVECTOR VEC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&ownerPos));
        DirectX::XMFLOAT3 vec{};
        DirectX::XMStoreFloat3(&vec, VEC);

        // そもそも範囲外なので判定を行わない
        float vecX = vec.x;
        vecX = (vecX > 0) ? vecX : -vecX;
        if (vecX > 4.0f)return;

        // owner:進行方向右,player:右の範囲内にいる
        if (owner->GetMoveRight() && vec.x > 0)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Find));
        }
        // owner:進行方向左,player:左の範囲内にいる
        if (!owner->GetMoveRight() && vec.x < 0)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Find));
        }
    }
}

// TurnState
namespace TOFU
{
    // 初期化
    void TurnState::Enter()
    {
        // 回転速度設定
        SetRotationSpeed(3.0f);

        // 回転方向設定
        rotate = owner->GetMoveRight() ? 270.0f : 90.0f;
    }

    // 更新
    void TurnState::Execute(float elapsedTime)
    {
        // 次に進む方向へ回転する
        {
            DirectX::XMFLOAT4 rotation = owner->GetTransform()->GetRotation();
            // 90度の場合
            if (rotate == 90.0f)
            {
                // 回転
                rotation.y -= GetRotationSpeed() * elapsedTime;

                // 回転終わったらAttackStateへ
                if (DirectX::XMConvertToRadians(rotate) >= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(90.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Walk));
                }
            }
            // 270度の場合
            else
            {
                // 回転
                rotation.y += GetRotationSpeed() * elapsedTime;

                // 回転終わったらAttackStateへ
                if (DirectX::XMConvertToRadians(rotate) <= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(270.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Walk));
                }
            }
            owner->GetTransform()->SetRotation(rotation);
        }
    }

    // 終了
    void TurnState::Exit()
    {
        owner->SetMoveRight(owner->GetMoveRight() ? false : true);
    }
}

// FindState
namespace TOFU
{
    // 初期化
    void FindState::Enter()
    {
        // 黄色
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));

        // 今いる地点Yを保存する
        returnPositionY = owner->GetTransform()->GetPosition().y;

        // stateをリセット
        state = 0;

        // 飛び跳ねる速度を設定する
        SetMoveSpeed(10.0f);
    }

    // 更新
    void FindState::Execute(float elapsedTime)
    {
        DirectX::XMFLOAT3 pos = owner->GetTransform()->GetPosition();
        switch (state)
        {
        case 0:
            pos.y += GetMoveSpeed() * elapsedTime;
            if (pos.y > returnPositionY + 2.0f)state = 1;
            break;
        case 1:
            pos.y -= GetMoveSpeed() * elapsedTime;
            if (pos.y < returnPositionY)
                owner->GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Track));
            break;
        }
        owner->GetTransform()->SetPosition(pos);
    }

    // 終了
    void FindState::Exit()
    {

    }
}

// TrackState
namespace TOFU
{
    // 初期化
    void TrackState::Enter()
    {
        // 赤色
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

        // 速度を設定
        SetMoveSpeed(2.0f);

        // 回転速度を設定
        SetRotationSpeed(10.0f);
    }

    // 更新
    void TrackState::Execute(float elapsedTime)
    {
        // 追跡するために位置をとる
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        DirectX::XMVECTOR VEC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&ownerPos));
        DirectX::XMFLOAT3 vec{};
        DirectX::XMStoreFloat3(&vec, VEC);

        // 追跡の方向に会うための回転
        DirectX::XMFLOAT4 ownerRot = owner->GetTransform()->GetRotation();

        if (vec.x > 0)
        {
            ownerPos.x += GetMoveSpeed() * elapsedTime;
            
            // 右に進むので回転Yを90度になるように設定する
            if (ownerRot.y != DirectX::XMConvertToRadians(90))
            {
                ownerRot.y -= GetRotationSpeed() * elapsedTime;
                
                // 90度より小さくなったら90度に設定する
                if (ownerRot.y < DirectX::XMConvertToRadians(90))
                    ownerRot.y = DirectX::XMConvertToRadians(90);
            }
        }
        else
        {
            ownerPos.x -= GetMoveSpeed() * elapsedTime;

            // 左に進むので回転Yを270度になるように設定する
            if (ownerRot.y != DirectX::XMConvertToRadians(270))
            {
                ownerRot.y += GetRotationSpeed() * elapsedTime;

                // 270度より大きくなったら270度に設定する
                if (ownerRot.y > DirectX::XMConvertToRadians(270))
                    ownerRot.y = DirectX::XMConvertToRadians(270);
            }
        }

        owner->GetTransform()->SetPosition(ownerPos);
        owner->GetTransform()->SetRotation(ownerRot);
    }

    // 終了
    void TrackState::Exit()
    {
    }
}

// IdleState
namespace CANNON
{
    // 初期化
    void IdleState::Enter()
    {
        // materialColor(青)
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

        // タイマーセット
        SetTimer(3.0f);
    }

    // 更新
    void IdleState::Execute(float elapsedTime)
    {
        // タイマーが0になったら
        if (GetTimer() < 0)owner->GetStateMachine()->ChangeState(static_cast<int>(CANNON::STATE::Attack));

        // タイマーを減らす
        SubtractTime(elapsedTime);
    }

    // 終了
    void IdleState::Exit()
    {

    }
}

// AttackState
namespace CANNON
{
    // 初期化
    void AttackState::Enter()
    {
        // materialColorを設定(攻撃(赤))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    // 更新   
    void AttackState::Execute(float elapsedTime)
    {
        // todo:ここで弾丸を生成する
        //CannonBall* cannonBall = new CannonBall(owner->GetTransform()->GetPosition(), -1, &owner->cannonBallManager);

        // IdleStateへ
        owner->GetStateMachine()->ChangeState(static_cast<int>(CANNON::STATE::Idle));
    }

    // 終了
    void AttackState::Exit()
    {

    }
}