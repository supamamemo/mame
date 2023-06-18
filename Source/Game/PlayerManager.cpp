#include "PlayerManager.h"


void PlayerManager::Initialize()
{
    player->Initialize();
}

void PlayerManager::Finalize()
{
    player->Finalize();
}

void PlayerManager::Begin()
{
    player->Begin();
}

void PlayerManager::Update(const float& elapsedTime)
{
    player->Update(elapsedTime);
}

void PlayerManager::End()
{
    player->End();
}

void PlayerManager::Render(const float& elapsedTime)
{
    player->Render(elapsedTime);
}

void PlayerManager::DrawDebug()
{
    player->DrawDebug();
}
