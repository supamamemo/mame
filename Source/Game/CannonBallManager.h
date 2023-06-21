#pragma once

#include <vector>
#include <set>
#include "CannonBall.h"

class CannonBallManager
{
public:
    CannonBallManager();
    ~CannonBallManager();

    // XVˆ—
    void Update(float elapsedTime);

    // •`‰æˆ—
    void Render(float elapsedTime);

    void DrawDebug();

public:
    // ’eŠÛ“o˜^
    void Register(CannonBall* cannonBall);

    // ’eŠÛ‘Síœ
    void Clear();

    // ’eŠÛ”æ“¾
    int GetCannonBallCount() { return static_cast<int>(balls.size()); }

    // ’eŠÛæ“¾
    CannonBall* GetCannonBall(int index) { return balls.at(index); }

    // ’eŠÛíœ
    void Remove(CannonBall* cannonBall);

private:
    std::vector<CannonBall*> balls;
    std::set<CannonBall*> removes;
};

