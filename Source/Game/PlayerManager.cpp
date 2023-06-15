#include "PlayerManager.h"

void PlayerManager::Update(const float& elapsedTime)
{
    player->Update(elapsedTime);
}

void PlayerManager::Render(const float& elapsedTime)
{
    player->Render(elapsedTime);
}