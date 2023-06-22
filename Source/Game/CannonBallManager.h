#pragma once

#include <vector>
#include <set>
#include "CannonBall.h"


class CannonBallManager
{
public:
    CannonBallManager();
    ~CannonBallManager();

    void Update(const float& elapsedTime);  // �X�V����   
    void Render(const float& elapsedTime);  // �`�揈��
    void DrawDebug();                       // �f�o�b�O�`�揈��

public:  
    void Register(CannonBall* cannonBall);  // �e�ۓo�^
    void Clear();                           // �e�ۑS�폜  
    void Remove(CannonBall* cannonBall);    // �e�ۍ폜

public:
    // �e�ې��擾
    const int GetCannonBallCount() const { return static_cast<int>(cannonballs.size()); }

    // �e�ێ擾
    CannonBall* GetCannonBall(const int& index) const { return cannonballs.at(index); }

private:
    std::vector<CannonBall*> cannonballs    = {};
    std::set<CannonBall*>    removes        = {};
};

