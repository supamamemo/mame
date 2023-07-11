#include "Enemy.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "../Mame/Scene/SceneManager.h"
#include "../Mame/AudioManager.h"

float Enemy::renderLengthXLimit_ = 20.0f;
float Enemy::playSELengthXLimit_ = 10.0f;


void Enemy::Render(const float& elapsedTime)
{
    // DrawColl�����Ȃ����邽�߂�player����߂����̂����`�悷��
    if (!IsInLengthPlayer(renderLengthXLimit_)) return;

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
    // ����������ł���Ƃ��͏������s��Ȃ�
    if (this->health <= 0) return;

    const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();
    if (Collision::IntersectAABBVsAABB(this->aabb_, player->aabb_))
    {    
        // �_���[�W���󂯂Ȃ����return
        if (!player->ApplyDamage(1, 2.0f)) return;

        this->OnAttacked(); // �U�������Ƃ��ɌĂ΂�鏈��

        isHitPlayer_ = true; // �v���C���[�ɓ�������

        AudioManager& audioManager = AudioManager::Instance();
        audioManager.PlaySE(SE::PL_Damaged, false);   // �v���C���[�̔�_���[�WSE�Đ�
    }
}


// �v���C���[���������ɂ��邩���肷��
bool Enemy::IsInLengthPlayer(const float lengthXLimit)
{
    const float enemyPosX  = GetTransform()->GetPosition().x;
    const float playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
    const float lengthX    = fabsf(enemyPosX - playerPosX);
    
    if (lengthX > lengthXLimit)
    {
        return false;
    }

    return true;
}


bool Enemy::Turn(
    const    float elapsedTime, 
    const    float vx, 
    NO_CONST float turnSpeed)
{
    Transform* transform = GetTransform();

    turnSpeed *= elapsedTime;

    // ���g�̉�]�l����O���������߂�
    const float frontX = sinf(transform->GetRotation().y);

    // ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��όv�Z����
    const float dot = (frontX * vx);

    // ��]�p�������ȏꍇ�͉�]���s��Ȃ�(��]�I��)
    const float angle = acosf(dot); // ���W�A��
    //if (fabsf(angle) <= 0.001f) return false;
    if (fabsf(angle) <= 0.05f) return false;

    // ���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����  
    NO_CONST float rot = 1.0f - dot;   // �␳�l                    
    if (rot > turnSpeed) rot = turnSpeed;

    // ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    const float frontZ = cosf(transform->GetRotation().y);
    const float cross  = (frontZ * vx);

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    transform->AddRotationY((cross < 0.0f) ? -rot : rot);

    return true;    // ��]��
}
