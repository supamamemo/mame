#include "Camera.h"

#include "../Input/Input.h"

//// 透視投影
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
//// 平行投影
//void Camera::Update(ID3D11DeviceContext* dc, DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 focus, DirectX::XMFLOAT3 up, float viewWidth, float viewHeight, float nearZ, float farZ)
//{
//}


void Camera::SetPerspectiveFov(ID3D11DeviceContext* dc)
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    dc->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,100.0f) };
    
    
    DirectX::XMVECTOR eye{ DirectX::XMVectorSet(camera.eye.x,camera.eye.y,camera.eye.z,1.0f) };
    DirectX::XMVECTOR focus{ DirectX::XMVectorSet(camera.focus.x,camera.focus.y,camera.focus.z,1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(camera.up.x,camera.up.y,camera.up.z,0.0f) };
    V = { DirectX::XMMatrixLookAtLH(eye, focus, up) };


}

void Camera::DrawDebug()
{
    ImGui::Begin("camera");

    if (ImGui::TreeNode("camera_information"))
    {
        ImGui::DragFloat3("eye", &camera.eye.x);
        ImGui::DragFloat3("focus", &camera.focus.x);
        ImGui::DragFloat3("up", &camera.up.x);
        ImGui::TreePop();
    }

    // todo : camera
    // デバッグ用にカメラを動かしやすいようにしている
    ImGui::DragFloat("camera_position.x", &camera.eye.x);
    ImGui::DragFloat("camera_position.y", &camera.eye.y);
    DebugMoveCamera();
    if (ImGui::Button("Reset"))Camera::Reset();

    ImGui::End();
}

void Camera::Reset()
{
    camera.eye = { 0.0f,0.0f,-10.0f };
}

void Camera::DebugMoveCamera()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButton() & GamePad::BTN_UP)camera.eye.y += 0.01f;
    if (gamePad.GetButton() & GamePad::BTN_DOWN)camera.eye.y -= 0.01f;
    if (gamePad.GetButton() & GamePad::BTN_RIGHT)camera.eye.x += 0.01f;
    if (gamePad.GetButton() & GamePad::BTN_LEFT)camera.eye.x -= 0.01f;

    camera.focus.x = camera.eye.x;
    camera.focus.y = camera.eye.y;
}
