#include "CannonBallManager.h"

// コンストラクタ
CannonBallManager::CannonBallManager()
{
}

// デストラクタ
CannonBallManager::~CannonBallManager()
{
    Clear();
}

// 更新処理
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

// 描画処理
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

// 弾丸登録
void CannonBallManager::Register(CannonBall* cannonBall)
{
    balls.emplace_back(cannonBall);
}

// 弾丸全削除
void CannonBallManager::Clear()
{
    for (auto& cannonBall : balls)
    {
        delete cannonBall;
    }
}

// 弾丸削除
void CannonBallManager::Remove(CannonBall* cannonBall)
{
    removes.insert(cannonBall);
}
