#pragma once

#include <DirectxMath.h>

// レンダーコンテキスト
struct RenderContext
{
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
    DirectX::XMFLOAT4   lightDirection;
};