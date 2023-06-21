#include "Enemy.h"

void Enemy::PlayAnimation(const int& index, const bool& loop)
{
    Character::PlayAnimation(index, loop);
}

void Enemy::UpdateAnimation(const float& elapsedTime)
{
    Character::UpdateAnimation(elapsedTime);
}

bool Enemy::IsPlayAnimation() const
{
    return Character::IsPlayAnimation();
}
