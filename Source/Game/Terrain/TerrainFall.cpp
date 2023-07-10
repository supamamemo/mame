#include "TerrainFall.h"
#include "../PlayerManager.h"

TerrainFall::TerrainFall(const char* const filename)
    : Terrain(filename)
{
}

void TerrainFall::Initialize()
{
    Terrain::Initialize();

    terrainType_ = Terrain::Type::Move; // �����n�`�^�C�v�ɐݒ�

    SetMaterialColor(DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f));
}

void TerrainFall::Update(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    // �����t���O�������Ă���ꍇ�͗����������s��
    if (isFall_)
    {
        velocity_.y += (fallAcceleration_ * elapsedTime);   // �������x�����Z
        if (velocity_.y < fallVelocityMax_) velocity_.y = fallVelocityMax_;
        transform->AddPositionY(velocity_.y);               // ����

        // ���܂ŗ��������珉���ʒu�ɖ߂��ă��Z�b�g����
        if (transform->GetPosition().y < -20.0f)
        {
            transform->SetPositionY(originPosition_.y);
            velocity_.y = 0.0f;
            isFall_     = false;
            blinkTimer_ = defaultBlinkTime_;

            // �v���C���[���ۑ����Ă���n�`���̒n�`�������������炻�̒n�`��������
            // (���Z�b�g���Ƀv���C���[�����Ă��Ȃ��悤�ɂ��邽��)
            PlayerManager& playerManager = PlayerManager::Instance();
            if (this == playerManager.GetPlayer()->saveTerrain_)
            {
                playerManager.GetPlayer()->saveTerrain_ = nullptr;
            }
        }
    }

    // �_�Ń^�C�}�[���ݒ肳��Ă���Γ_�ł�����
    if (blinkTimer_ > 0.0f)
    {
        blinkTimer_ -= elapsedTime;

        if (blinkTimer_ <= 0.0f)
        {
            blinkTimer_     = 0.0f;
            materialColor   = { 0.7f,0.7f,0.7f,1 };
        }
        else if (static_cast<int>(blinkTimer_ * 100.0f) & 0x08)
        {
            materialColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        }
        else
        {
            materialColor = { 0.7f,0.7f,0.7f,1 };
        }
    }

    UpdateAABB();   // AABB�̍X�V����
}


void TerrainFall::DrawDebug()
{
    ImGui::Begin(GetName());
    Terrain::DrawDebug();
    ImGui::End();
}


void TerrainFall::OnRiding()
{
    isFall_ = true;     // �����J�n
}
