#include "Camera.h"

#include "../Input/Input.h"

#include "../../Game/PlayerManager.h"

void Camera::Initialize()
{
    transform.SetPosition({ 0, 21.0f, -30 });
    transform.SetRotation({});
}


void Camera::Update(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();

    // プレイヤーが死んでいたらreturn
    if (playerManager.GetPlayer()->GetHealth() <= 0) return;

    // プレイヤーのxにカメラのxをあわせる
    {
        DirectX::XMFLOAT3 playerPos = playerManager.GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 cameraPos = GetTransform()->GetPosition();

        //if (cameraPos.x < playerPos.x)
        //{
        //    cameraPos.x += 4.f * elapsedTime;
        //    if (cameraPos.x > playerPos.x) cameraPos.x = playerPos.x;
        //}
        //else if (cameraPos.x > playerPos.x)
        //{
        //    cameraPos.x += -4.f * elapsedTime;
        //    if (cameraPos.x < playerPos.x) cameraPos.x = playerPos.x;
        //}
        cameraPos.x = playerPos.x;

        // todo : ここどうにかしたい
        // 高いところ
        if (playerPos.x < 37.0f)
            cameraMoveY = 0;
        if ((playerPos.x > 37.0f && playerPos.x < 60.0f) && playerPos.y > 8.0f)
            cameraMoveY = 1;
        if (playerPos.x > 60.0f)
            cameraMoveY = 0;
        if (playerPos.x > 95.5f && playerPos.y > 7.4f)
            cameraMoveY = 2;
        if (playerPos.x > 140.0f && playerPos.y > 7.0f)
            cameraMoveY = 1;


        switch (cameraMoveY)
        {
        case 0:
            cameraPos.y -= elapsedTime * 5;
            if (cameraPos.y <= 8.0f)cameraPos.y = 8.0f;
            break;
        case 1:
            cameraPos.y += elapsedTime * 5;
                if (cameraPos.y >= 11.0f)cameraPos.y = 11.0f;
            break;
        case 2:
            cameraPos.y += elapsedTime * 5;
            if (cameraPos.y >= 10.0f)cameraPos.y = 10.0f;
            break;
        }

        GetTransform()->SetPosition(cameraPos);
    }
}


void Camera::UpdateTitle(float elapsedTime)
{
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();

    DirectX::XMFLOAT3 vec = pos;
    vec.x = sinf(angle);
    vec.z = cosf(angle);

    vec.x*= length;
    vec.z*= length;

    angle -= addAngle * elapsedTime;

    GetTransform()->SetPosition(vec);
}

void Camera::UpdateTutorial(float elapsedTime, int state)
{
    DirectX::XMFLOAT3 cameraPos = GetTransform()->GetPosition();

    float leftLimit = 0.0f;

    switch (state)
    {
    case STATE::MoveReception:
        leftLimit = -10.4f;
        break;
    case STATE::JumpReception:
        leftLimit = -10.4f;
        break;
    case STATE::HipDorop:
        leftLimit = 0.0f;

        cameraPos.x += elapsedTime * 10;
        if (cameraPos.x >= 9.0f)cameraPos.x = 9.0f;
        break;
    default:
        leftLimit = -10.4f;
        break;
    }

    GetTransform()->SetPosition(cameraPos);

    // 画面左端は止まるようにする
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        if (playerPos.x <= leftLimit)playerPos.x = leftLimit;

        PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(playerPos);
    }
}


void Camera::UpdateBoss(const float elapsedTime)
{
    UpdateShake(elapsedTime);   // 画面振動更新

    DirectX::XMFLOAT3 cameraPos = GetTransform()->GetPosition();
    DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();

    if (playerPos.x >= -10.0f)
        cameraMoveY = 1;

    switch (cameraMoveY)
    {
    case 1:
        cameraPos.x += elapsedTime * 5;
        if (cameraPos.x >= 0.0f)cameraPos.x = 0.0f;

        break;
    }

    GetTransform()->SetPosition(cameraPos);
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

void Camera::SetPerspectiveFovTitle(ID3D11DeviceContext* dc)
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    dc->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,1000.0f) };

    DirectX::XMFLOAT3 pos = transform.GetPosition();
    DirectX::XMFLOAT3 forward = transform.CalcForward();
    DirectX::XMVECTOR eye{ DirectX::XMVectorSet(pos.x,pos.y,pos.z,1.0f) };
    DirectX::XMVECTOR focus{ DirectX::XMVectorSet(0.0f,2.0f,0.0f,0.0f) };
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
    
    ImGui::DragFloat("angle", &angle);
    ImGui::DragFloat("addAngle", &addAngle);
    ImGui::DragFloat("length", &length);

    ImGui::DragInt("cameraMoveY", &cameraMoveY);


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



#ifdef _DEBUG
void Camera::SetDebugCamera()
{
    DirectX::XMFLOAT3 pos = transform.GetPosition();
    pos.y = 1.3f;
    transform.SetPosition(pos);
    length = 40.0f;
}

void Camera::SetDebugCamera1()
{
    DirectX::XMFLOAT3 pos = transform.GetPosition();
    pos.y = 21.0f;
    transform.SetPosition(pos);
    length = 60.0f;
}
#endif // _DEBUG

void Camera::UpdateShake(const float elapsedTime)
{
    if (!isShake_) return; // 画面振動していなければreturn

    shakeTimer_ -= elapsedTime;
    if (shakeTimer_ > 0.0f) return;
    else shakeTimer_ = defaultShakeTime_;

    const float posX = 0.25f;
    const float posY = 0.25f;
    switch (shakeType_)
    {
    case ShakeType::HorizontalShake:
#if 1
        switch (shakeState_)
        {
        case 0:
            transform.SetPosition(DirectX::XMFLOAT3(posX, 10.0f, -12.0f));
            ++shakeState_;
            break;
        case 1:
            transform.SetPosition(DirectX::XMFLOAT3(-posX, 10.0f, -12.0f));
            ++shakeState_;
            break;
        case 2:
            // 指定ループ回数までループ
            if (currentShakeLoopCount_ < shakeLoopCount_)
            {
                shakeState_ = 0;
                ++currentShakeLoopCount_;
                break;
            }

            // シェイク終了
            transform.SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
            shakeType_              = ShakeType::None;
            shakeState_             = 0;
            currentShakeLoopCount_  = 0;
            shakeLoopCount_         = 0;
            isShake_                = false;
            break;
        }
        break;
#endif
        break;
    case ShakeType::VerticalShake:
#if 1
        switch (shakeState_)
        {
        case 0:
            transform.SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f + -posY, -12.0f));
            ++shakeState_;
            break;
        case 1:
            transform.SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f + posY, -12.0f));
            ++shakeState_;
            break;
        case 2:
            // 指定ループ回数までループ
            if (currentShakeLoopCount_ < shakeLoopCount_)
            {
                shakeState_ = 0;
                ++currentShakeLoopCount_;
                break;
            }

            // シェイク終了
            transform.SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));

            shakeType_              = ShakeType::None;
            shakeState_             = 0;
            currentShakeLoopCount_  = 0;
            shakeLoopCount_         = 0;
            isShake_                = false;
            break;
        }
        break;
#endif
    default:
        isShake_ = false;
        break;
    }

}

