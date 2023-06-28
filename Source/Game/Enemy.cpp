#include "Enemy.h"
#include "PlayerManager.h"

float Enemy::renderLengthXLimit_ = 40.0f;

void Enemy::Render(const float& elapsedTime)
{
    // DrawColl‚ð­‚È‚­‚·‚é‚½‚ß‚Éplayer‚©‚ç‹ß‚¢‚à‚Ì‚¾‚¯•`‰æ‚·‚é
    {
        const float& enemyPosX  = GetTransform()->GetPosition().x;
        const float& playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float vecX        = enemyPosX - playerPosX;
        const float lengthX     = sqrtf(vecX * vecX);

        if (lengthX > renderLengthXLimit_) return;
    }

    Character::Render(elapsedTime);
}
