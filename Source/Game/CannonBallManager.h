#pragma once

#include <vector>
#include <set>
#include "CannonBall.h"

class CannonBallManager
{
public:
    CannonBallManager();
    ~CannonBallManager();

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(float elapsedTime);

    void DrawDebug();

public:
    // �e�ۓo�^
    void Register(CannonBall* cannonBall);

    // �e�ۑS�폜
    void Clear();

    // �e�ې��擾
    int GetCannonBallCount() { return static_cast<int>(balls.size()); }

    // �e�ێ擾
    CannonBall* GetCannonBall(int index) { return balls.at(index); }

    // �e�ۍ폜
    void Remove(CannonBall* cannonBall);

private:
    std::vector<CannonBall*> balls;
    std::set<CannonBall*> removes;
};

