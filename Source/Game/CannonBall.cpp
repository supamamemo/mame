#include "CannonBall.h"

#include "../Mame/Graphics/Graphics.h"

#include "CannonBallManager.h"

int CannonBall::nameNum = 0;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="newPosition">��C�̈ʒu</param>
/// <param name="directionZ">�C�e�̂����ޕ���Z</param>
CannonBall::CannonBall(DirectX::XMFLOAT3 newPosition, float directionZ, CannonBallManager* manager)
    :manager(manager),
    directionZ(directionZ)
{
    manager->Register(this);

    Graphics& graphics = Graphics::Instance();

    // model�ǂݍ���
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_cannon_bullet.fbx", true);

    // �������W���C�̈ʒu���班���O�ɂ���
    {
        offsetZ = (directionZ > 0) ? offsetZ : -offsetZ;
        newPosition.z += offsetZ;
        GetTransform()->SetPosition(newPosition);
    }

    // imgui���O���Ԃ�N���Ȃ��悤��...
    {
        name = "Boss" + std::to_string(nameNum);
        SetName(name.c_str());
        ++nameNum;
    }
}

// �f�X�g���N�^
CannonBall::~CannonBall()
{
}

// ������
void CannonBall::Initialize()
{
}

// �I����
void CannonBall::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void CannonBall::Begin()
{
}

// �X�V����
void CannonBall::Update(const float& elapsedTime)
{
    float speed = (directionZ > 0) ? moveSpeed * elapsedTime : -moveSpeed * elapsedTime;

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    pos.z += speed;
    GetTransform()->SetPosition(pos);
}

// Update�̌�ɌĂ΂��
void CannonBall::End()
{
}

// �`�揈��
void CannonBall::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // Transform�X�V
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��
    if (&keyframe)
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), &keyframe);
    }
    else
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    }

#if _DEBUG
    // BOUNDING_BOX
    {
        DirectX::XMFLOAT4X4 t = SetDebugModelTransform(transform);
        debugModel->skinned_meshes.render(graphics.GetDeviceContext(), t, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.2f), nullptr);
    }
#endif // _DEBUG
}

// imgui
void CannonBall::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    Character::DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}

// �j��
void CannonBall::Destroy()
{
    manager->Remove(this);
}
