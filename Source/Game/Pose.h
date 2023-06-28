#pragma once

#include "../sprite.h"
#include <memory>

class Pose
{
public:
    Pose(const wchar_t* filename);
    ~Pose();

    void Render();

    bool IsIndication() { return isIndication; }
    void SetIndication(bool indication) { isIndication = indication; }
    
    void SetMaterialColor(DirectX::XMFLOAT4 color) { materialColor = color; }
    DirectX::XMFLOAT4 GetMaterialColor() { return materialColor; }

private:
    std::unique_ptr<Sprite>sprite;

    DirectX::XMFLOAT4 materialColor{ 1.0f,1.0f,1.0f,1.0f };

    bool isIndication = true;

    struct Spr
    {
        DirectX::XMFLOAT2 pos{};
        DirectX::XMFLOAT2 texPos{ 1280,720 };
    }spr;
};

