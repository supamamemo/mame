#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../Transform.h"

class Camera
{
public:
    Camera() {}
    ~Camera() {}

    void SetPerspectiveFov(ID3D11DeviceContext* dc);
    //void SetPerspectiveFov(ID3D11DeviceContext* dc, float fovY, float aspect, float nearZ, float farZ);
   
    DirectX::XMMATRIX GetP() { return P; }
    DirectX::XMMATRIX GetV() { return V; }

    void DrawDebug();

    struct view_camera
    {
        DirectX::XMFLOAT3 eye{ 0.0f,0.0f,-10.0f };
        DirectX::XMFLOAT3 focus{ 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 up{ 0.0f,1.0f,0.0f };
    }camera;

    void Reset();
    void DebugMoveCamera();

private:
    Transform transform{};

    DirectX::XMMATRIX P{};
    DirectX::XMMATRIX V{};

    //DirectX::XMFLOAT4X4 view;
    //DirectX::XMFLOAT4X4 projection;
    //DirectX::XMFLOAT3 eye;
    //DirectX::XMFLOAT3 focus;

    //DirectX::XMFLOAT3 up;
    //DirectX::XMFLOAT3 front;
    //DirectX::XMFLOAT3 right;
};

