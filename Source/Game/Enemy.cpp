#include "Enemy.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

float Enemy::renderLengthXLimit_ = 40.0f;


void Enemy::Render(const float& elapsedTime)
{
    // DrawColl�����Ȃ����邽�߂�player����߂����̂����`�悷��
    {
        const float& enemyPosX  = GetTransform()->GetPosition().x;
        const float& playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float vecX        = enemyPosX - playerPosX;
        const float lengthX     = sqrtf(vecX * vecX);

        if (lengthX > renderLengthXLimit_) return;
    }

    Character::Render(elapsedTime);
}


// �������E�����~�X�����Ƃ��ɌĂ΂��
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
        // �_���[�W���󂯂Ȃ����return
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

    // ���g�̉�]�l����O���������߂�
    const float frontX = sinf(rotation.y);
    const float frontZ = cosf(rotation.y);

    // ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��όv�Z����
    const float dot = (frontX * vx) /*+ (frontZ * vz)*/;

    // ���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����  
    NO_CONST float rot = 1.0f - dot;   // �␳�l                    
    if (rot > turnSpeed) rot = turnSpeed;

    // ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    const float cross = (frontZ * vx) /*- (frontX * vz)*/;

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    rotation.y += (cross < 0.0f) ? -rot : rot;

    GetTransform()->SetRotation(rotation);
}
