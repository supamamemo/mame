#include "Enemy.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

float Enemy::renderLengthXLimit_ = 40.0f;


void Enemy::Render(const float& elapsedTime)
{
    // DrawCollを少なくするためにplayerから近いものだけ描画する
    {
        const float& enemyPosX  = GetTransform()->GetPosition().x;
        const float& playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float vecX        = enemyPosX - playerPosX;
        const float lengthX     = sqrtf(vecX * vecX);

        if (lengthX > renderLengthXLimit_) return;
    }

    Character::Render(elapsedTime);
}


// 落下死・落下ミスしたときに呼ばれる
void Enemy::OnFallDead()
{
    Destroy();
}


void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}


void Enemy::CollisionEnemyVsPlayer()
{    
    const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();
    if (Collision::IntersectAABBVsAABB(this->aabb_, player->aabb_))
    {    
        // ダメージを受けなければreturn
        if (!player->ApplyDamage(1, 1.0f)) return;
    }
}

void Enemy::Turn(
    const    float elapsedTime, 
    const    float vx, 
    NO_CONST float turnSpeed)
{
    NO_CONST DirectX::XMFLOAT4 rotation = GetTransform()->GetRotation();

    turnSpeed *= elapsedTime;

    // 自身の回転値から前方向を求める
    const float frontX = sinf(rotation.y);
    const float frontZ = cosf(rotation.y);

    // 回転角を求めるため、2つの単位ベクトルの内積計算する
    const float dot = (frontX * vx) /*+ (frontZ * vz)*/;

    // 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
    // 小さいほど1.0に近づくという性質を利用して回転速度を調整する  
    NO_CONST float rot = 1.0f - dot;   // 補正値                    
    if (rot > turnSpeed) rot = turnSpeed;

    // 左右判定を行うために2つの単位ベクトルの外積を計算する
    const float cross = (frontZ * vx) /*- (frontX * vz)*/;

    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    // 左右判定を行うことによって左右回転を選択する
    rotation.y += (cross < 0.0f) ? -rot : rot;

    GetTransform()->SetRotation(rotation);
}
