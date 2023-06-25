#include "Camera.h"

#include "../Input/Input.h"

#include "../../Game/PlayerManager.h"

void Camera::Initialize()
{
    transform.SetPosition({ 0, 1.3f, -30 });
    transform.SetRotation({});
}

void Camera::Update(float elapsedTime)
{
    // プレイヤーのxにカメラのxをあわせる
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 cameraPos = GetTransform()->GetPosition();

        cameraPos.x = playerPos.x;
        GetTransform()->SetPosition(cameraPos);
    }
}

void Camera::UpdateTitle(float elapsedTime)
{
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    DirectX::XMFLOAT4 rot = GetTransform()->GetRotation();

    float r = 24;
    float move = 8;

    switch (state)
    {
    case 0:
        pos.x += move * elapsedTime;
        pos.z += move * elapsedTime;

        rot.y -= DirectX::XMConvertToRadians(r) * elapsedTime;

        if (rot.y <= DirectX::XMConvertToRadians(-90))rot.y = DirectX::XMConvertToRadians(-90);
        if (pos.x >= 30.0f)
        {
            pos.x = 30.0f;
            pos.z = 0.0f;
            rot.y = DirectX::XMConvertToRadians(-90);
            state = 1;
        }

        break;
    case 1:
        pos.x -= move * elapsedTime;
        pos.z += move * elapsedTime;

        rot.y -= DirectX::XMConvertToRadians(r) * elapsedTime;

        if (rot.y <= DirectX::XMConvertToRadians(-180))rot.y = DirectX::XMConvertToRadians(-180);
        if (pos.x <= 0.0f)
        {
            pos.x = 0.0f;
            pos.z = 30.0f;
            rot.y = DirectX::XMConvertToRadians(-180);
            state = 2;
        }
        break;
    case 2:
        pos.x -= move * elapsedTime;
        pos.z -= move * elapsedTime;

        rot.y -= DirectX::XMConvertToRadians(r) * elapsedTime;

        if (rot.y <= DirectX::XMConvertToRadians(-270))rot.y = DirectX::XMConvertToRadians(-270);
        if (pos.x <= -30.0f)
        {
            pos.x = -30.0f;
            pos.z = 0.0f;
            rot.y = DirectX::XMConvertToRadians(-270);
            state = 3;
        }
        break;
    case 3:
        pos.x += move * elapsedTime;
        pos.z -= move * elapsedTime;

        rot.y -= DirectX::XMConvertToRadians(r) * elapsedTime;

        if (rot.y <= DirectX::XMConvertToRadians(-360))rot.y = DirectX::XMConvertToRadians(-360);
        if (pos.x >= 0.0f)
        {
            pos.x = 0.0f;
            pos.z = -30.0f;
            rot.y = DirectX::XMConvertToRadians(0);
            state = 0;
        }
        break;
    }

    GetTransform()->SetPosition(pos);
    GetTransform()->SetRotation(rot);
}


void Camera::SetPerspectiveFov(ID3D11DeviceContext* dc)
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    dc->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,1000.0f) };
    
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
