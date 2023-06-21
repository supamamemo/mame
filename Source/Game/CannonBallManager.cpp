#include "CannonBallManager.h"

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
void CannonBallManager::Update(float elapsedTime)
{
    for (auto& cannonBall : balls)
    {
        cannonBall->Update(elapsedTime);
    }

    for (CannonBall* cannonBall : removes)
    {
        std::vector<CannonBall*>::iterator it = std::find(balls.begin(), balls.end(), cannonBall);

        if (it != balls.end())
        {
            balls.erase(it);
        }

        delete cannonBall;
    }
    removes.clear();
}

// �`�揈��
void CannonBallManager::Render(float elapsedTime)
{
    for (auto& cannonBall : balls)
    {
        cannonBall->Render(elapsedTime);
    }
}

void CannonBallManager::DrawDebug()
{
    for (auto& cannonBall : balls)
    {
        cannonBall->DrawDebug();
    }
}

// �e�ۓo�^
void CannonBallManager::Register(CannonBall* cannonBall)
{
    balls.emplace_back(cannonBall);
}

// �e�ۑS�폜
void CannonBallManager::Clear()
{
    for (auto& cannonBall : balls)
    {
        delete cannonBall;
    }
}

// �e�ۍ폜
void CannonBallManager::Remove(CannonBall* cannonBall)
{
    removes.insert(cannonBall);
}
