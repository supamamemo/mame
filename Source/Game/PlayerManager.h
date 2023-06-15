#pragma once

#include "Player.h"

class PlayerManager
{
private:
    PlayerManager()  {}
    ~PlayerManager() {}

public:
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    void Update(const float& elapsedTime);
    //void Render(NO_CONST ID3D11DeviceContext*& dc, NO_CONST Shader*& shader);
    void Render(const float& elapsedTime);

    // ƒvƒŒƒCƒ„[æ“¾
    NO_CONST std::unique_ptr<Player>& GetPlayer() { return player; }

private:
    std::unique_ptr<Player> player = nullptr;
};

