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


void Camera::Initialize()
{
    transform.SetPosition({});
    transform.SetRotation({});
}

void Camera::SetPerspectiveFov(ID3D11DeviceContext* dc)
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    dc->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,100.0f) };
    
    DirectX::XMFLOAT3 pos = transform.GetPosition();
    DirectX::XMFLOAT3 forward = transform.CalcForward();
    DirectX::XMVECTOR eye{ DirectX::XMVectorSet(pos.x,pos.y,pos.z,1.0f) };
    DirectX::XMVECTOR focus{ DirectX::XMVectorSet(pos.x + forward.x,pos.y + forward.y,pos.z + forward.z,1.0f) };
    //DirectX::XMVECTOR eye{ DirectX::XMVectorSet(camera.eye.x,camera.eye.y,camera.eye.z,1.0f) };
    //DirectX::XMVECTOR focus{ DirectX::XMVectorSet(camera.focus.x,camera.focus.y,camera.focus.z,1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(camera.up.x,camera.up.y,camera.up.z,0.0f) };
    V = { DirectX::XMMatrixLookAtLH(eye, focus, up) };


    //DebugMoveCamera();
}

void Camera::DrawDebug()
{
    ImGui::Begin("C_T");

    transform.DrawDebug();

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

    DirectX::XMFLOAT3 pos = transform.GetPosition();
    DirectX::XMFLOAT3 forward = transform.CalcForward();
    DirectX::XMFLOAT3 right = transform.CalcRight();

    float ax = 0;
    float ay = 0;

    //if (gamePad.GetButton() & GamePad::BTN_UP)ay = 0.01f;
    //if (gamePad.GetButton() & GamePad::BTN_DOWN)ay = -0.01f;
    //if (gamePad.GetButton() & GamePad::BTN_RIGHT)ax = 0.01f;
    //if (gamePad.GetButton() & GamePad::BTN_LEFT)ax = -0.01f;

    ax = gamePad.GetAxisLX() * 0.01f;
    ay = gamePad.GetAxisLY() * 0.01f;

    forward.x *= ay;
    forward.y *= ay;
    forward.z *= ay;
    right.x *= ax;
    right.y *= ax;
    right.z *= ax;

    pos.x += forward.x + right.x;
    //pos.y += forward.y + right.y;
    pos.z += forward.z + right.z;

    // プレイヤー移動確認のためにコメントアウトしています
    //transform.SetPosition(pos);
}
