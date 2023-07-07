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

    // プレイヤーとの距離Yが索敵距離Yより遠ければfalse
    const float lengthY = fabsf(playerPos.y - ownerPos.y);
    if (lengthY > owner->GetSerchLengthY()) return false;

    // プレイヤーとの距離Xが索敵距離Xより遠ければfalse
    const float lengthX = fabsf(playerPos.x - ownerPos.x);
    if (lengthX > owner->GetSerchLengthX()) return false;

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
        // materialColorを設定(セーフティー(青))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0, 0, 1, 0.4f));

        // 待機時間をセット
        SetTimer(2.0f);

        // アニメーションセット
        owner->PlayAnimation(static_cast<int>(BossAnimation::Idle), true);
    }

    // 更新
    void IdleState::Execute(float elapsedTime)
    {
        // 待機時間を減少
        SubtractTime(elapsedTime);

        // 待機時間が終了したら発見ステートへ遷移
        if (GetTimer() <= 0.0f)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
            return;
        }

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
        // materialColor(見つけた(黄色))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 0.4f));

        // タイマーをセット
        SetTimer(1.0f);

        // アニメーションセット
        owner->PlayAnimation(static_cast<int>(BossAnimation::AttackBefore), true);
    }

    // 更新
    void FindState::Execute(float elapsedTime)
    {
        // 時間を減らす
        SubtractTime(elapsedTime);

        // タイマーが0になったら旋回ステートへ遷移
        if (GetTimer() < 0.0f)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
            return;
        }

        owner->CollisionEnemyVsPlayer();    // プレイヤーとの衝突判定処理
    }

    // 終了
    void FindState::Exit()
    {
    }
}

// TurnState
namespace BOSS
{
    // 初期化
    void TurnState::Enter()
    {
        // materialColor(回転(緑))
        owner->SetMaterialColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.4f));

        // 回転速度設定
        //SetRotationSpeed(3.0f);

        // プレイヤーがどっちにいるのか
        //rotate = playerPos.x > ownerPos.x ? 90.0f : 270.0f;
    
        // 進む方向を設定する
        //owner->SetMoveRight(playerPos.x > ownerPos.x ? true : false);

        // プレイヤーがどっちにいるのか
        const DirectX::XMFLOAT3& playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();
        const float vecX    = (playerPos.x - ownerPos.x);
        const float vecX_n  = (vecX / fabsf(vecX));

        // 進む方向を設定する
        owner->SetMoveDirectionX(vecX_n);
    }

    // 更新
    void TurnState::Execute(float elapsedTime)
    {
        // プレイヤーの方向に向くように回転する
        {
            //DirectX::XMFLOAT4 rotation = owner->GetTransform()->GetRotation();
            //// 90度の場合
            //if (rotate == 90.0f)
            //{
            //    // 回転
            //    //rotation.y -= GetRotationSpeed() * elapsedTime;
            //    
            //    // 回転終わったらAttackStateへ
            //    if (DirectX::XMConvertToRadians(rotate) >= rotation.y)
            //    {
            //        rotation.y = DirectX::XMConvertToRadians(90.0f);
            //        owner->GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Attack));
            //    }
            //}
            //// -90度の場合
            //else
            //{
            //    // 回転
            //    //rotation.y += GetRotationSpeed() * elapsedTime;

            //    // 回転終わったらAttackStateへ
            //    if (DirectX::XMConvertToRadians(rotate) <= rotation.y)
            //    {
            //        rotation.y = DirectX::XMConvertToRadians(270.0f);
            //        owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
            //    }
            //}
            //owner->GetTransform()->SetRotation(rotation);
        }
        // 回転が終わったら攻撃ステートへ遷移
        const float moveDirectionX = owner->GetMoveDirectionX();
        const float turnSpeed      = owner->GetTurnSpeed();      
        if (!owner->Turn(elapsedTime, moveDirectionX, turnSpeed)) 
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Attack));
            return;
        }

        owner->CollisionEnemyVsPlayer();    // プレイヤーとの衝突判定処理
    }

    // 終了
    void TurnState::Exit()
    {
    }
}

// AttackState
namespace BOSS
{
    // 初期化
    void AttackState::Enter()
    {
        // materialColorを設定(アグレッシブ(赤))
        owner->SetMaterialColor(DirectX::XMFLOAT4(1, 0, 0, 0.4f));

        // 左右判定
        //owner->SetMoveSpeed(owner->GetMoveRight() ? speed : -speed);
        
        // アニメーションセット
        owner->PlayAnimation(static_cast<int>(BossAnimation::Attack), true);
    }

    // 更新
    void AttackState::Execute(float elapsedTime)
    {
        //const DirectX::XMFLOAT3& ownerPos = owner->GetTransform()->GetPosition();
        Transform* transform = owner->GetTransform();
        
        // 移動
        //ownerPos.x += owner->GetMoveSpeed() * elapsedTime;
        owner->SetMoveSpeed(owner->GetMoveDirectionX() * speed);
        transform->AddPositionX(owner->GetMoveSpeed() * elapsedTime);

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
        // materialColorを設定(紫)
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 0.4f));
        
        // 現在の反動距離をリセット
        currentRecoilLength = 0.0f;
        
        //SetRecoil(owner->GetStateMachine()->GetMoveRight() ? 1.0f : -1.0f);
        //SetRecoil(owner->GetMoveDirectionX());

        // 無敵状態を無くす
        owner->SetIsInvincible(false);

        // アニメーションセット
        owner->PlayAnimation(static_cast<int>(BossAnimation::Recoil), true);
    }

    // 更新
    void RecoilState::Execute(float elapsedTime)
    {
        Transform* transform = owner->GetTransform();

        //owner->SetMoveSpeed(owner->GetStateMachine()->GetMoveRight() ? speed : -speed);

        //if (owner->GetStateMachine()->GetMoveRight())
        if (currentRecoilLength < recoilLength)
        {
            owner->SetMoveSpeed(owner->GetMoveDirectionX() * speed);

            transform->AddPositionX(owner->GetMoveSpeed() * elapsedTime);
            currentRecoilLength += fabsf(owner->GetMoveSpeed() * elapsedTime);

            if (currentRecoilLength >= recoilLength) SetTimer(1.0f);
        }
        else
        {
            SubtractTime(elapsedTime);
            if (GetTimer() <= 0.0f) owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Idle));
        }
        
        //owner->GetTransform()->SetPosition(ownerPos);

        // 当たり判定
        if(Collision::IntersectAABBVsAABB(PlayerManager::Instance().GetPlayer()->aabb_, owner->aabb_))
        { 
           // todo : 当たり判定 
        }
    }

    // 終了
    void RecoilState::Exit()
    {
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
        Transform* transform = owner->GetTransform();                

        // 向いている方向に移動
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // playerが索敵範囲にいたら発見ステートへ遷移
        if (FindPlayer())
        {
            // 地面か敵の上に乗っていたら遷移させるようにする(空中ジャンプ防止)
            if (owner->GetIsGround() || owner->GetIsOnFriend())
            {
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
                return;
            }
        }

        // 地形の端を超えそうになったら旋回ステートへ遷移
        {
            if (owner->lastLandingTerrainAABB_.max.x != 0.0f && 
                owner->aabb_.max.x > owner->lastLandingTerrainAABB_.max.x)
            {
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // 移動方向を反転
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
            else if (owner->lastLandingTerrainAABB_.min.x != 0.0f &&
                owner->aabb_.min.x < owner->lastLandingTerrainAABB_.min.x)
            {
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // 移動方向を反転
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }

        // 右方向に向いている場合、目的地も左側にあるので到達しているか判定する
        if (owner->GetMoveDirectionX() == 1.0f)
        {
            // 目的地を超えていたら旋回ステートへ遷移
            if (transform->GetPosition().x >= owner->GetDestination())
            {
                // ※追跡の移動などで目的地を大幅に超えていた場合、位置修正が瞬間移動になってしまうのでコメントアウトにしている
                //transform->SetPositionX(owner->GetDestination());       // 位置修正             
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // 移動方向を反転
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }
        // 左方向に向いている場合、目的地も左側にあるので到達しているか判定する
        else if (owner->GetMoveDirectionX() == -1.0f)
        {
            // 目的地を超えていたら旋回ステートへ遷移
            if (transform->GetPosition().x <= owner->GetDestination())
            {
                // ※追跡の移動などで目的地を大幅に超えていた場合、位置修正が瞬間移動になってしまうのでコメントアウトにしている
                //transform->SetPositionX(owner->GetDestination());       // 位置修正              
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
        
        //// 回転が終わった後、範囲内にプレイヤーがいたら発見ステートへ遷移
        //if (State::FindPlayer())
        //{
        //    owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
        //    return;
        //}
        //// 歩行ステートへ遷移
        //else
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

        owner->SetVelocityX(0.0f); // X速度をリセット
        owner->Move(0.0f, 0.0f);   // 横移動入力リセット

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
        case 0: // ジャンプさせる
            owner->Jump(owner->GetJumpSpeed());
            owner->SetIsOnFriend(false); // 味方の上に乗っかっているかフラグをリセットする
            ++state;
            break;
        case 1: // 地面か敵の上についたら追跡ステートへ遷移
            if (owner->GetIsGround() || owner->GetIsOnFriend())
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
        SetTimer(3.0f);

        owner->PlayAnimation(TofuAnimation::Walk, true);
        owner->SetAnimationSpeed(1.25f); // アニメーション速度を速めに設定
    }

    // 更新
    void TrackState::Execute(float elapsedTime)
    {
        const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();

        // プレイヤーへ向かう方向を算出して移動方向に設定
        {
            const DirectX::XMFLOAT3& playerPos = player->GetTransform()->GetPosition();
            const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();
            const float vecX      = (playerPos.x - ownerPos.x);
            const float lengthX   = fabsf(vecX);
            // 距離がゼロ以下だと豆腐が消滅するので防止
            if (lengthX > 0.0f)
            {
                const float vecX_n = (vecX / lengthX);
                owner->SetMoveDirectionX(vecX_n);
            }
        }

        // プレイヤーを追いかける
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // 旋回処理
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());

        // プレイヤーが索敵範囲から外れたら追跡時間を減少し、
        // 追跡時間が終わったら旋回ステートへ遷移する(通常時に戻る)
        if (player->GetHealth() <= 0 || !FindPlayer())
        {
            SubtractTime(elapsedTime);
            if (GetTimer() <= 0.0f)
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
            SetTimer(3.0f);
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
        SetTimer(3.0f);

        // 再生停止
        owner->PlayAnimation(TofuAnimation::Turn, true, 1.25f);
    }

    void IdleBattleState::Execute(float elapsedTime)
    {
        // 旋回処理
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());
        
        SubtractTime(elapsedTime); // 待機時間減少

        if (GetTimer() > 0.0f) return;

        // プレイヤーが範囲内にいたら発見ステートへ遷移
        if (FindPlayer())
        {
            // 地面か敵の上に乗っていたら遷移させるようにする(空中ジャンプ防止)
            if (owner->GetIsGround() || owner->GetIsOnFriend())
            {
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
                return;
            }
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

// DeathState(死亡ステート)
namespace TOFU
{
    void DeathState::Enter()
    {
        owner->SetMoveSpeed(10.0f);             // XZ方向の吹っ飛び力を設定

        owner->SetJumpSpeed(20.0f);             // Y方向の吹っ飛び力設定
        owner->Jump(owner->GetJumpSpeed());     // 上に吹っ飛ばす

        owner->SetTurnSpeed(ToRadian(540.0f)); // 回転速度設定
    }

    void DeathState::Execute(float elapsedTime)
    {
        Transform* transform = owner->GetTransform();

        // 吹っ飛ばす
        const float moveDirectionX  = owner->GetMoveDirectionX();           // 吹っ飛ぶ方向X(OnDead関数で設定済み)
        const float impulse         = owner->GetMoveSpeed() * elapsedTime;  // 吹っ飛び力
        const float velocityX       = moveDirectionX * impulse;             // 横に吹っ飛ばす
        const float velocityZ       = -impulse;                             // 手前に吹っ飛ばす
        transform->AddPosition(DirectX::XMFLOAT3(velocityX, 0.0f, velocityZ));

        // 回転させる
        const float rotate = moveDirectionX * owner->GetTurnSpeed() * elapsedTime;
        transform->AddRotationX(rotate);
        transform->AddRotationZ(rotate);

        // (消滅処理はOnFallDead関数で行っている)
    }

    void DeathState::Exit()
    {
    }
}



// WalkState
namespace RED_TOFU
{
    // 初期化
    void WalkState::Enter()
    {
        // 移動速度設定
        owner->SetMoveSpeed(2.0f);

        // 加速力設定
        owner->SetAcceleration(1.0f);

        // 摩擦力設定
        owner->SetFriction(0.5f);

        // 旋回速度設定
        owner->SetTurnSpeed(ToRadian(180.0f));

        // アニメーション設定
        owner->PlayAnimation(TofuAnimation::Walk, true);
    }

    // 更新
    void WalkState::Execute(float elapsedTime)
    {
        Transform* transform = owner->GetTransform();

        // 向いている方向に移動
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // playerが索敵範囲にいたら発見ステートへ遷移
        if (FindPlayer())
        {
            // 地面か敵の上に乗っていたら遷移させるようにする(空中ジャンプ防止)
            if (owner->GetIsGround() || owner->GetIsOnFriend())
            {
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
                return;
            }
        }

        // 地形の端を超えそうになったら旋回ステートへ遷移
        {
            if (owner->lastLandingTerrainAABB_.max.x != 0.0f &&
                owner->aabb_.max.x > owner->lastLandingTerrainAABB_.max.x)
            {
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // 移動方向を反転
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
            else if (owner->lastLandingTerrainAABB_.min.x != 0.0f &&
                owner->aabb_.min.x < owner->lastLandingTerrainAABB_.min.x)
            {
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // 移動方向を反転
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }

        // 右方向に向いている場合、目的地も左側にあるので到達しているか判定する
        if (owner->GetMoveDirectionX() == 1.0f)
        {
            // 目的地を超えていたら旋回ステートへ遷移
            if (transform->GetPosition().x >= owner->GetDestination())
            {
                // ※追跡の移動などで目的地を大幅に超えていた場合、位置修正が瞬間移動になってしまうのでコメントアウトにしている
                //transform->SetPositionX(owner->GetDestination());       // 位置修正             
                owner->SetMoveDirectionX(-owner->GetMoveDirectionX());  // 移動方向を反転
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Turn));
                return;
            }
        }
        // 左方向に向いている場合、目的地も左側にあるので到達しているか判定する
        else if (owner->GetMoveDirectionX() == -1.0f)
        {
            // 目的地を超えていたら旋回ステートへ遷移
            if (transform->GetPosition().x <= owner->GetDestination())
            {
                // ※追跡の移動などで目的地を大幅に超えていた場合、位置修正が瞬間移動になってしまうのでコメントアウトにしている
                //transform->SetPositionX(owner->GetDestination());       // 位置修正              
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
namespace RED_TOFU
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


        // 地形の端を超えて落ちそうになったら修正
        {
            if (owner->lastLandingTerrainAABB_.max.x != 0.0f && owner->aabb_.max.x > owner->lastLandingTerrainAABB_.max.x)
            {
                const float fixLeft = -fabsf(owner->aabb_.max.x - owner->lastLandingTerrainAABB_.max.x);
                owner->GetTransform()->AddPositionX(fixLeft);
            }
            else if (owner->lastLandingTerrainAABB_.min.x != 0.0f && owner->aabb_.min.x < owner->lastLandingTerrainAABB_.min.x)
            {
                const float fixRight = fabsf(owner->aabb_.min.x - owner->lastLandingTerrainAABB_.min.x);
                owner->GetTransform()->AddPositionX(fixRight);
            }
        }

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
namespace RED_TOFU
{
    // 初期化
    void FindState::Enter()
    {
        // 黄色
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 0.4f));

        owner->SetVelocityX(0.0f); // X速度をリセット
        owner->Move(0.0f, 0.0f);   // 横移動入力リセット

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
        case 0: // ジャンプさせる
            owner->Jump(owner->GetJumpSpeed());
            owner->SetIsOnFriend(false); // 味方の上に乗っかっているかフラグをリセットする
            ++state;
            break;
        case 1: // 地面か敵の上についたら追跡ステートへ遷移
            if (owner->GetIsGround() || owner->GetIsOnFriend())
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
namespace RED_TOFU
{
    // 初期化
    void TrackState::Enter()
    {
        // 赤色
        owner->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.4f));

        // 速度を設定
        owner->SetMoveSpeed(4.0f);

        // 加速力設定(弱めに設定)
        owner->SetAcceleration(0.7f);        
        
        // 摩擦力設定(弱めに設定)
        owner->SetFriction(0.1f);

        // 回転速度を設定
        owner->SetTurnSpeed(ToRadian(540.0f));

        //// 追跡時間を設定
        //SetTimer(3.0f);

        owner->PlayAnimation(TofuAnimation::Walk, true);
        owner->SetAnimationSpeed(1.5f); // アニメーション速度を速めに設定
    }

    // 更新
    void TrackState::Execute(float elapsedTime)
    {
        const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();

        if (player->GetHealth() <= 0)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::IdleBattle));
        }

        // プレイヤーへ向かう方向を算出して移動方向に設定
        {
            const DirectX::XMFLOAT3& playerPos = player->GetTransform()->GetPosition();
            const DirectX::XMFLOAT3& ownerPos  = owner->GetTransform()->GetPosition();
            const float vecX      = (playerPos.x - ownerPos.x);
            const float lengthX   = fabsf(vecX);
            // 距離がゼロ以下だと豆腐が消滅するので防止
            if (lengthX > 0.0f)
            {
                const float vecX_n = (vecX / lengthX);
                owner->SetMoveDirectionX(vecX_n);
            }
        }

        // プレイヤーを追いかける
        owner->Move(owner->GetMoveDirectionX(), owner->GetMoveSpeed());

        // 旋回処理
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());

    }

    // 終了
    void TrackState::Exit()
    {
    }
}

// IdleBattleState(攻撃後にしばらく待機するステート)
namespace RED_TOFU
{
    void IdleBattleState::Enter()
    {
        // 移動速度を設定(停止)
        owner->SetMoveSpeed(0.0f);

        // 戦闘待機時間を設定
        SetTimer(3.0f);

        // 再生停止
        owner->PlayAnimation(TofuAnimation::Turn, true, 1.25f);
    }

    void IdleBattleState::Execute(float elapsedTime)
    {
        // 旋回処理
        owner->Turn(elapsedTime, owner->GetMoveDirectionX(), owner->GetTurnSpeed());
        
        SubtractTime(elapsedTime); // 待機時間減少

        if (GetTimer() > 0.0f) return;

        // プレイヤーが生きていたら発見ステートへ遷移
        const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();
        if (player->GetHealth() > 0)
        {
            // 地面か敵の上に乗っていたら遷移させるようにする(空中ジャンプ防止)
            if (owner->GetIsGround() || owner->GetIsOnFriend())
            {
                owner->GetStateMachine()->ChangeState(static_cast<int>(STATE::Find));
                return;
            }
        }
        // プレイヤーが死んでいたら旋回ステートへ遷移(通常時に戻る)
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

// DeathState(死亡ステート)
namespace RED_TOFU
{
    void DeathState::Enter()
    {
        owner->SetMoveSpeed(10.0f);             // XZ方向の吹っ飛び力を設定

        owner->SetJumpSpeed(20.0f);             // Y方向の吹っ飛び力設定
        owner->Jump(owner->GetJumpSpeed());     // 上に吹っ飛ばす

        owner->SetTurnSpeed(ToRadian(540.0f)); // 回転速度設定
    }

    void DeathState::Execute(float elapsedTime)
    {
        Transform* transform = owner->GetTransform();

        // 吹っ飛ばす
        const float moveDirectionX  = owner->GetMoveDirectionX();           // 吹っ飛ぶ方向X(OnDead関数で設定済み)
        const float impulse         = owner->GetMoveSpeed() * elapsedTime;  // 吹っ飛び力
        const float velocityX       = moveDirectionX * impulse;             // 横に吹っ飛ばす
        const float velocityZ       = -impulse;                             // 手前に吹っ飛ばす
        transform->AddPosition(DirectX::XMFLOAT3(velocityX, 0.0f, velocityZ));

        // 回転させる
        const float rotate = moveDirectionX * owner->GetTurnSpeed() * elapsedTime;
        transform->AddRotationX(rotate);
        transform->AddRotationZ(rotate);

        // (消滅処理はOnFallDead関数で行っている)
    }

    void DeathState::Exit()
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