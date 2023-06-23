#pragma once

#include "../Mame/Graphics/Model.h"

class Block
{
public:
    Block();
    virtual ~Block() {}

    virtual void DrawDebug();

    DirectX::XMFLOAT4X4 SetDebugModelTransform(DirectX::XMFLOAT4X4 transform);

public:
    std::unique_ptr<Model> model = nullptr;
    std::unique_ptr<Model> debugModel = nullptr;
};