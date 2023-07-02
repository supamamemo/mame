#include "CannonBall.h"
#include "CannonBallManager.h"

#include "../Mame/Graphics/Graphics.h"
#include "PlayerManager.h"

int   CannonBall::nameNum             = 0;
float CannonBall::renderLengthXLimit_ = 40.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="newPosition">��C�̈ʒu</param>
/// <param name="directionZ">�C�e�̂����ޕ���Z</param>
CannonBall::CannonBall(
    NO_CONST DirectX::XMFLOAT3 newPosition, 
    const float& directionZ, 
    NO_CONST CannonBallManager* manager)
    : cannonBallManager(manager)
    , directionZ(directionZ)
{
    cannonBallManager->Register(this);

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
    // ��������
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0)
    {
        // ����������
        Destroy();
        return;
    }

    float speed = (directionZ > 0) ? moveSpeed_ * elapsedTime : -moveSpeed_ * elapsedTime;

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    pos.z += speed;
    GetTransform()->SetPosition(pos);

    UpdateAABB();    // AABB�X�V����
}

// Update�̌�ɌĂ΂��
void CannonBall::End()
{
}

// �`�揈��
void CannonBall::Render(const float& elapsedTime)
{
    // DrawColl�����Ȃ����邽�߂�player����߂����̂����`�悷��
    {
        const float& enemyPosX  = GetTransform()->GetPosition().x;
        const float& playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float  lengthX    = fabsf(enemyPosX - playerPosX);

        if (lengthX > renderLengthXLimit_) return;
    }

    Character::Render(elapsedTime);
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
    cannonBallManager->Remove(this);
}
