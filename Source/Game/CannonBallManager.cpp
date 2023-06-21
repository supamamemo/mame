#include "CannonBallManager.h"
#include "Common.h"

// �R���X�g���N�^
CannonBallManager::CannonBallManager()
{
}

// �f�X�g���N�^
CannonBallManager::~CannonBallManager()
{
    Clear();
}

// �X�V����
void CannonBallManager::Update(const float& elapsedTime)
{
    for (CannonBall*& cannonBall : cannonballs)
    {
        cannonBall->Update(elapsedTime);
    }

    for (CannonBall* cannonBall : removes)
    {
        std::vector<CannonBall*>::iterator it = std::find(cannonballs.begin(), cannonballs.end(), cannonBall);

        if (it != cannonballs.end())
        {
            cannonballs.erase(it);
        }

        SafeDelete(cannonBall);
    }
    removes.clear();
}

// �`�揈��
void CannonBallManager::Render(const float& elapsedTime)
{
    for (CannonBall*& cannonBall : cannonballs)
    {
        cannonBall->Render(elapsedTime);
    }
}

void CannonBallManager::DrawDebug()
{
    for (CannonBall*& cannonBall : cannonballs)
    {
        cannonBall->DrawDebug();
    }
}

// �e�ۓo�^
void CannonBallManager::Register(CannonBall* cannonBall)
{
    cannonballs.emplace_back(cannonBall);
}

// �e�ۑS�폜
void CannonBallManager::Clear()
{
    for (CannonBall*& cannonBall : cannonballs)
    {
        SafeDelete(cannonBall);
    }
    cannonballs.clear();
    cannonballs.shrink_to_fit();
}

// �e�ۍ폜
void CannonBallManager::Remove(CannonBall* cannonBall)
{
    removes.insert(cannonBall);
}
