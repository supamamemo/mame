#include "Camera.h"

//// “§Ž‹“Š‰e
//void Camera::Update(ID3D11DeviceContext* dc, DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 focus, DirectX::XMFLOAT3 up, ID3D11Buffer* CbSceneBuffer)
//{
//    //D3D11_VIEWPORT viewport{};
//    //UINT unm_viewports{ 1 };
//    //dc->RSGetViewports(&unm_viewports, &viewport);
//
//    //float aspect_ratio{ viewport.Width / viewport.Height };
//    //DirectX::XMMATRIX P{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,100.0f) };
//
//    //Eye = DirectX::XMLoadFloat3(&eye);
//    //Focus = DirectX::XMLoadFloat3(&focus);
//    //Up = DirectX::XMLoadFloat3(&up);
//    //V = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
//
//    //CbScene data{};
//    //DirectX::XMStoreFloat4x4(&data.viewProjection, V * P);
//    //data.lightDirection = { 0,0,1,0 };
//    //data.camera_position = { 0,0,1,0 };
//
//    //dc->UpdateSubresource(CbSceneBuffer, 0, 0, &data, 0, 0);
//    //dc->VSSetConstantBuffers(1, 1, &CbSceneBuffer);
//    //dc->PSSetConstantBuffers(1, 1, &CbSceneBuffer);
//
//}
//
//// •½s“Š‰e
//void Camera::Update(ID3D11DeviceContext* dc, DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 focus, DirectX::XMFLOAT3 up, float viewWidth, float viewHeight, float nearZ, float farZ)
//{
//}


void Camera::SetPerspectiveFov(ID3D11DeviceContext* dc)
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    dc->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    P={ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,100.0f) };
    
    
    DirectX::XMVECTOR eye{ DirectX::XMVectorSet(pos.x,pos.y,pos.z,1.0f) };
    DirectX::XMVECTOR focus{ DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f) };
    V={ DirectX::XMMatrixLookAtLH(eye, focus, up) };


}

void Camera::DrawDebug()
{
    ImGui::Begin("camera");
    ImGui::DragFloat3("pos", &pos.x);
    ImGui::End();
}
