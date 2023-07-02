#include "Boss.h"
#include "BossStateDerived.h"

#include "PlayerManager.h"

#include "CannonBall.h"
#include "EnemyTofu.h"


// playerが索敵範囲内にいるか判定する
bool State::FindPlayer()
{
    const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();

    if (player->GetHealth() <= 0) return false; // プレイヤーが死んでいたらfalse

    const DirectX::XMFLOAT3& playerPos = player->GetTransform()->GetPosition();
    const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();

    // プレイヤーとの距離が索敵距離より遠ければfalse
    const float length = fabsf(playerPos.x - ownerPos.x);
    if (length > owner->GetSerchLength()) return false;

    // 右を向いている状態でプレイヤーが自分より右にいるか、
    // 左を向いている状態でプレイヤーが自分より左にいたら発見しているのでtrue
    if ((owner->GetMoveDirectionX() ==  1.0f && playerPos.x > ownerPos.x) ||
        (owner->GetMoveDirectionX() == -1.0f && playerPos.x < ownerPos.x))
    {
        return true;
    }

    // 見つからなかった
    return false;
}

// IdleState
namespace BOSS
{
    // 初期化
    void IdleState::Enter()
    {
        // アニメーションセット
        owner->PlayAnimation(static_cast<int>(BossAnimation::Idle), true);
                
        // materialColorを設定(セーフティー(青))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 0.4f));

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

        owner->CollisionEnemyVsPlayer();    // プレイヤーとの衝突判定処理
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 0.4f));

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

        owner->CollisionEnemyVsPlayer();    // プレイヤーとの衝突判定処理
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.4f));

        // 回転速度設定
        //SetRotationSpeed(3.0f);

        // プレイヤーがどっちにいるのか
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer().get()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        rotate = playerPos.x > ownerPos.x ? 90.0f : 270.0f;
        
        // 進む方向を設定する
        owner->SetMoveRight(playerPos.x > ownerPos.x ? true : false);
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
                //rotation.y -= GetRotationSpeed() * elapsedTime;
                
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
                //rotation.y += GetRotationSpeed() * elapsedTime;

                // 回転終わったらAttackStateへ
                if (DirectX::XMConvertToRadians(rotate) <= rotation.y)
                {
                    rotation.y = DirectX::XMConvertToRadians(270.0f);
                    owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
                }
            }
            owner->GetTransform()->SetRotation(rotation);
        }

        owner->CollisionEnemyVsPlayer();    // プレイヤーとの衝突判定処理
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 0.4f));

        // 左右判定
        owner->SetMoveSpeed(owner->GetMoveRight() ? speed : -speed);
    }

    // 更新
    void AttackState::Execute(float elapsedTime)
    {
        // アニメーション更新
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();

        ownerPos.x += owner->GetMoveSpeed() * elapsedTime;

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

        owner->CollisionEnemyVsPlayer();    // プレイヤーとの衝突判定処理
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 0.4f));
        
        // 反動距離を設定
        recoilCount = 0;
        
        SetRecoil(owner->GetStateMachine()->GetMoveRight() ? 1.0f : -1.0f);

        // 無敵状態を無くす
        owner->SetIsInvincible(false);
    }

    // 更新
    void RecoilState::Execute(float elapsedTime)
    {
        // アニメーション更新
        owner->UpdateAnimation(elapsedTime);

        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        owner->SetMoveSpeed(owner->GetStateMachine()->GetMoveRight() ? speed : -speed);
        ownerPos.x += owner->GetMoveSpeed() * elapsedTime;
        recoilCount += owner->GetMoveSpeed() * elapsedTime;

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

        // 無敵状態にする
        owner->SetIsInvincible(true);
    }
}


// WalkState
namespace TOFU
{
    // 初期化
    void WalkState::Enter()
    {
        // 移動速度設定
        owner->SetMoveSpeed(1.0f);        

        // 旋回速度設定
        owner->SetTurnSpeed(ToRadian(90.0f));

        // アニメーション設定
        owner->PlayAnimation(TofuAnimation::Walk, true);
    }

    // 更新
    void WalkState::Execute(float elapsedTime)
    {
        Transform*   transform      = owner->GetTransform();                
        const float  moveDirectionX = owner->GetMoveDirectionX();           
        const float  velocityX      = (owner->GetMoveSpeed() * elapsedTime);

        // 向いている方向に移動
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // playerが索敵範囲にいたら発見ステートへ遷移
        if (FindPlayer())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
            return;
        }

        // 右方向に向いている場合、目的地も左側にあるので到達しているか判定する
        if (moveDirectionX == 1.0f)
        {
            // 目的地を超えていたら旋回ステートへ遷移
            if (transform->GetPosition().x >= owner->GetDestination())
            {
                transform->SetPositionX(owner->GetDestination());       // 位置修正             
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // 移動方向を反転
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }
        // 左方向に向いている場合、目的地も左側にあるので到達しているか判定する
        else if (moveDirectionX == -1.0f)
        {
            // 目的地を超えていたら旋回ステートへ遷移
            if (transform->GetPosition().x <= owner->GetDestination())
            {
                transform->SetPositionX(owner->GetDestination());       // 位置修正             
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // 移動方向を反転
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }        
    }

    // 終了
    void WalkState::Exit()
    {
    }
}

// TurnState
namespace TOFU
{
    // 初期化
    void TurnState::Enter()
    {
        // 旋回アニメーション設定
        owner->PlayAnimation(TofuAnimation::Turn, true);
    }

    // 更新
    void TurnState::Execute(float elapsedTime)
    {
        const float moveDirectionX = owner->GetMoveDirectionX();
        const float turnSpeed      = owner->GetTurnSpeed();

        // 回転中ならreturn
        if (owner->Turn(elapsedTime, moveDirectionX, turnSpeed)) return;
        
        // 回転が終わった後、範囲内にプレイヤーがいたら発見ステートへ遷移
        if (State::FindPlayer())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
            return;
        }
        // 歩行ステートへ遷移
        else
        {
            // 移動範囲の中心から移動方向に向かって移動範囲の半径分進んだ位置を目的地に設定する
            const float moveRangeCenterX = owner->GetMoveRangeCenterX();
            const float moveRangeRadius  = owner->GetMoveRangeRadius();
            owner->SetDestination(moveRangeCenterX + (owner->GetMoveDirectionX() * moveRangeRadius));

            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Walk));
            return;
        }      
    }

    // 終了
    void TurnState::Exit()
    {
    }
}

// FindState
namespace TOFU
{
    // 初期化
    void FindState::Enter()
    {
        // 黄色
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 0.4f));

        // stateをリセット
        state = 0;

        // アニメーション設定
        owner->PlayAnimation(TofuAnimation::Walk, true);
    }

    // 更新
    void FindState::Execute(float elapsedTime)
    {
        switch (state)
        {
        case 0: // 地面についていたらジャンプさせる(空中ジャンプ防止)
            if (owner->GetIsGround())
            {
                owner->Jump(owner->GetJumpSpeed());
                ++state;
                break;
            }

            break;
        case 1: // 地面についたら追跡ステートへ遷移
            if (owner->GetIsGround())
            {
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Track));
                break;
            }

            break;
        }
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.4f));

        // 速度を設定
        owner->SetMoveSpeed(2.0f);

        // 回転速度を設定
        owner->SetTurnSpeed(ToRadian(540.0f));

        // 追跡時間を設定
        State::SetTimer(3.0f);

        owner->PlayAnimation(TofuAnimation::Walk, true);
        owner->SetAnimationSpeed(1.25f); // アニメーション速度を速めに設定
    }

    // 更新
    void TrackState::Execute(float elapsedTime)
    {
        // プレイヤーへ向かう方向を算出して移動方向に設定
        {
            const DirectX::XMFLOAT3& playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
            const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();
            const float              vec       = (playerPos.x - ownerPos.x);
            const float              vec_n     = (vec / fabs(vec));
            if (vec_n != owner->GetMoveDirectionX()) owner->SetMoveDirectionX(vec_n);
        }

        // プレイヤーを追いかける
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // 旋回処理
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());

        // プレイヤーが索敵範囲から外れたら追跡時間を減少し、
        // 追跡時間が終わったら旋回ステートへ遷移する(通常時に戻る)
        if (!State::FindPlayer())
        {
            State::SubtractTime(elapsedTime);
            if (State::GetTimer() <= 0.0f)
            {
                // 移動方向の設定
                {
                    // 自分から移動範囲の中心へ向かう単位ベクトルを移動方向に設定
                    const float moveRangeCenterX = owner->GetMoveRangeCenterX();
                    const float positionX        = owner->GetTransform()->GetPosition().x;
                    const float vec              = (moveRangeCenterX - positionX);
                    const float vec_n            = (vec / fabsf(vec));
                    owner->SetMoveDirectionX(vec_n);
                }

                // 旋回速度を通常に戻す
                owner->SetTurnSpeed(ToRadian(90.0f));

                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }
        // プレイヤーが索敵範囲にいれば追跡時間を設定
        else
        {
            State::SetTimer(3.0f);
        }
    }

    // 終了
    void TrackState::Exit()
    {
    }
}

// IdleBattleState(攻撃後にしばらく待機するステート)
namespace TOFU
{
    void IdleBattleState::Enter()
    {
        // 移動速度を設定(停止)
        owner->SetMoveSpeed(0.0f);

        // 戦闘待機時間を設定
        State::SetTimer(3.0f);

        // 再生停止
        owner->PlayAnimation(-1, false);
    }

    void IdleBattleState::Execute(float elapsedTime)
    {
        // 旋回処理
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());
        
        State::SubtractTime(elapsedTime); // 待機時間減少

        if (State::GetTimer() > 0.0f) return;

        // プレイヤーが範囲内にいたら追跡ステートへ遷移
        if (State::FindPlayer())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Track));
            return;            
        }
        // プレイヤーが範囲内にいなくて待機時間が終了していれば旋回ステートへ遷移(通常時に戻る)
        else 
        {
            // 移動方向の設定
            {
                // 自分から移動範囲の中心へ向かう単位ベクトルを移動方向に設定
                const float moveRangeCenterX = owner->GetMoveRangeCenterX();
                const float positionX        = owner->GetTransform()->GetPosition().x;
                const float vec              = (moveRangeCenterX - positionX);
                const float vec_n            = (vec / fabsf(vec));
                owner->SetMoveDirectionX(vec_n);
            }

            // 旋回速度を通常に戻す
            owner->SetTurnSpeed(ToRadian(90.0f));

            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
            return;
        }
    }

    void IdleBattleState::Exit()
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
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.4f));

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
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.4f));
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