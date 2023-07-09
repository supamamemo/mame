#include "Camera.h"

#include "../Input/Input.h"

#include "../../Game/PlayerManager.h"
#include "../../Game/EnemyManager.h"

void Camera::Initialize()
{
    transform.SetPosition({ 0, 21.0f, -30 });
    transform.SetRotation({});
}


void Camera::Update(float elapsedTime)
{
#if 0
    PlayerManager& playerManager = PlayerManager::Instance();

    // �v���C���[������ł�����return
    if (playerManager.GetPlayer()->GetHealth() <= 0) return;

    // �v���C���[��x�ɃJ������x�����킹��
    {

        DirectX::XMFLOAT3 playerPos = playerManager.GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 cameraPos = GetTransform()->GetPosition();

        cameraPos.x = playerPos.x;

        // todo : �����ǂ��ɂ�������
        // �����Ƃ���
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

#else
    
    NO_CONST DirectX::XMFLOAT3& cameraPos = GetTransform()->GetPosition();

    NO_CONST PlayerManager&  playerManager = PlayerManager::Instance();
    const DirectX::XMFLOAT3& playerPos     = playerManager.GetPlayer()->GetTransform()->GetPosition();
    const float plLastLandingTerrainMaxY   = playerManager.GetPlayer()->lastLandingTerrainAABB_.max.y;

    // �J������X�ʒu���v���C���[��X�ʒu�Ɠ���
    if (playerManager.GetPlayer()->GetClearState() != ClearState::MoveToRight)
    {
        cameraPos.x = playerPos.x;
    }

    if (playerManager.GetPlayer()->GetState() != Player::State::Clear)
    {
#if 0
        const    float moveSpeedY      = 5.0f * elapsedTime;    // �ړ����xY
        NO_CONST float targetPositionY = 0.0f;                  // �ڕW�ʒuY

        // �v���C���[���Ō�ɒ��n�����n�`�̓��オ������Ȃ�J�������ړ�������
        if (plLastLandingTerrainMaxY > 7.5f)
        {
            // �ڕW�ʒu��ݒ�
            targetPositionY = plLastLandingTerrainMaxY + 1.75f;

            // �J�������ڕW�ʒu��艺�ɂ������Ɉړ�����
            if (cameraPos.y < targetPositionY)
            {
                // �J������ړ��E���ߏC��(�E������菬������ΉE����)
                cameraPos.y = (std::min)(targetPositionY, cameraPos.y + moveSpeedY);
            }

            // �J�������ڕW�ʒu����ɂ����牺�Ɉړ�����
            else if (cameraPos.y < targetPositionY)
            {
                // �J������ړ��E���ߏC��(�E�������傫����ΉE����)
                cameraPos.y = (std::max)(targetPositionY, cameraPos.y - moveSpeedY);
            }

        }
        // ����ȊO�Ȃ�J���������Ƃɖ߂�
        else
        {
            // �ڕW�ʒu��ݒ�
            targetPositionY = 8.0f;

            // �J�������ړ��E���ߏC��(�E�������傫����ΉE����)
            cameraPos.y = (std::max)(targetPositionY, cameraPos.y - moveSpeedY);
        }
#endif



        if (playerPos.x < 37.0f)coordinatesY = 8.0f;
        if ((playerPos.x > 31.0f && playerPos.x <= 36.7f)&& playerPos.y > 6.1f)coordinatesY = 9.0f;
        if ((playerPos.x > 36.7f && playerPos.x < 60.0f) && playerPos.y > 7.6f)coordinatesY = 11.0f;
        if ((playerPos.x > 60.1f && playerPos.x < 63.5f) && playerPos.y > 6.4f)coordinatesY = 10.0f;
        if (playerPos.x > 65.0f)coordinatesY = 8.0f;
        if ((playerPos.x > 98.0f && playerPos.y > 7.4f))coordinatesY = 10.0f;
        if ((playerPos.x > 121.0f && playerPos.x < 128.0f) && playerPos.y > 7.4f)coordinatesY = 10.0f;
        if ((playerPos.x > 140.0f && playerPos.x < 145.0f) && playerPos.y > 7.0f)coordinatesY = 10.0f;
        if ((playerPos.x > 223.0f) && playerPos.y > 7.0f)coordinatesY = 8.0f;
        

        if (cameraPos.y >= coordinatesY)
        {
            cameraPos.y -= elapsedTime * 5;
            if (cameraPos.y <= coordinatesY)cameraPos.y = coordinatesY;
        }
        else if (cameraPos.y < coordinatesY)
        {
            cameraPos.y += elapsedTime * 5;
            if (cameraPos.y >= coordinatesY)cameraPos.y = coordinatesY;
        }
        

    }
    // �v���C���[�̃X�e�[�g���N���A�X�e�[�g�Ȃ�J�������񂹂�
    else
    {
        const float targetPositionY = 3.5f;
        const float moveSpeedY      = 6.0f * elapsedTime;        
        if (cameraPos.y < targetPositionY)
        {
            cameraPos.y = (std::min)(targetPositionY, (cameraPos.y + moveSpeedY));
        }
        if (cameraPos.y > targetPositionY)
        {
            cameraPos.y = (std::max)(targetPositionY, (cameraPos.y - moveSpeedY));
        }        
        
        const float targetPositionZ = 0.0f;
        const float moveSpeedZ      = 6.0f * elapsedTime;
        if (cameraPos.z < targetPositionZ)
        {
            cameraPos.z = (std::min)(targetPositionZ, (cameraPos.z + moveSpeedZ));
        }
        if (cameraPos.z > targetPositionZ)
        {
            cameraPos.z = (std::max)(targetPositionZ, (cameraPos.z - moveSpeedZ));
        }
    }

    GetTransform()->SetPosition(cameraPos);
#endif

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
    DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();

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

        tutorialState = 0;
        break;
    case STATE::Run:
        leftLimit = 16.0f;

        switch (tutorialState)
        {
        case 0:
            cameraPos.x += elapsedTime * 10;
            if (cameraPos.x >= 25.0f)
            {
                cameraPos.x = 25.0f;
                tutorialState = 1;
            }
            break;
        case 1:
            if (playerPos.x >= 26.0f)
            {
                cameraPos.x += elapsedTime * 10;

                if (playerPos.x + 6.0f <= cameraPos.x)cameraPos.x = playerPos.x + 6.0f;
            }
            else
            {
                cameraPos.x -= elapsedTime * 10;
                if (cameraPos.x <= 25.0f)cameraPos.x = 25.0f;
            }
            break;
        }


        break;
    case STATE::Free:
        leftLimit = 35.0f;

        switch (tutorialState)
        {
        case 0:
            cameraPos.x += elapsedTime * 10;
            if (cameraPos.x >= 45.0f)
            {
                cameraPos.x = 45.0f;
                tutorialState = 1;
            }
            break;
        case 1:
            if (playerPos.x >= 45.0f)
            {
                cameraPos.x += elapsedTime * 10;
                if (cameraPos.x >= playerPos.x + 6.0f)cameraPos.x = playerPos.x + 6.0f;
            }
            else
            {
                cameraPos.x -= elapsedTime * 10;
                if (cameraPos.x <= 45.0f)cameraPos.x = 45.0f;
            }
            break;
        }

        break;
    case STATE::GoalState:
        cameraPos.x += elapsedTime * 10;

        if (cameraPos.x >= 63.0f)cameraPos.x = 63.0f;

        break;
    default:
        leftLimit = -10.4f;
        break;
    }

    GetTransform()->SetPosition(cameraPos);

    // ��ʍ��[�͎~�܂�悤�ɂ���
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        if (playerPos.x <= leftLimit)playerPos.x = leftLimit;

        PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(playerPos);
    }
}


void Camera::UpdateBoss(const float elapsedTime)
{
    UpdateShake(elapsedTime);   // ��ʐU���X�V

    DirectX::XMFLOAT3 cameraPos = GetTransform()->GetPosition();
    DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
    DirectX::XMFLOAT3 bossPos = EnemyManager::Instance().GetEnemy(0)->GetTransform()->GetPosition();

    float leftLimit = -30.0f;
    float rightLimit = -5.0f;

    switch (cameraMoveY)
    {
    case 0:
        GetTransform()->SetPosition(DirectX::XMFLOAT3(-20.0f, 10.0f, -12.0f));
        GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0.0f, 0.0f, 0.0f));

        if (playerPos.x >= -10.0f)
            cameraMoveY = 1;

        break;
    case 1:
        leftLimit = -10.0f;

        cameraPos.x += elapsedTime * 5;
        if (cameraPos.x >= 0.0f)
        {
            cameraPos.x = 0.0f;
            cameraMoveY = 2;
        }

        break;
    case 2:
        leftLimit = -10.0f;
        if (playerPos.x <= -8.6f)
            playerPos.x += elapsedTime;

        if (bossPos.x <= 5.5)cameraMoveY = 3;

        break;
    case 3:
        rightLimit = 100.0f;
        break;
    }

    GetTransform()->SetPosition(cameraPos);

    // ��ʍ��[�͎~�܂�悤�ɂ���
    {
        if (playerPos.x <= leftLimit)playerPos.x = leftLimit;
        if (playerPos.x >= rightLimit)playerPos.x = rightLimit;

        PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(playerPos);
    }
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
    // �f�o�b�O�p�ɃJ�����𓮂����₷���悤�ɂ��Ă���
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

    // �v���C���[�ړ��m�F�̂��߂ɃR�����g�A�E�g���Ă��܂�
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
    if (!isShake_) return; // ��ʐU�����Ă��Ȃ����return

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
            // �w�胋�[�v�񐔂܂Ń��[�v
            if (currentShakeLoopCount_ < shakeLoopCount_)
            {
                shakeState_ = 0;
                ++currentShakeLoopCount_;
                break;
            }

            // �V�F�C�N�I��
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
            // �w�胋�[�v�񐔂܂Ń��[�v
            if (currentShakeLoopCount_ < shakeLoopCount_)
            {
                shakeState_ = 0;
                ++currentShakeLoopCount_;
                break;
            }

            // �V�F�C�N�I��
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

