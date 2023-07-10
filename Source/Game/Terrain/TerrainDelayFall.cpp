#include "TerrainDelayFall.h"

#include "../PlayerManager.h"

TerrainDelayFall::TerrainDelayFall(const char* const filename)
    : Terrain(filename)
{
}

void TerrainDelayFall::Initialize()
{
    Terrain::Initialize();

    terrainType_ = Terrain::Type::Move; // �����n�`�^�C�v�ɐݒ�

    SetMaterialColor(DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f));
}

void TerrainDelayFall::Update(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    // ���������t���O�������Ă���ꍇ�͗����^�C�}�[���N������
    if (!isFall_ && isReadyFall_)
    {
        fallTimer_ += elapsedTime;

        // ��莞�Ԃ������痎���t���O�𗧂Ă�
        if (fallTimer_ >= 0.5f)
        {
            isFall_      = true;

            // �p�����[�^���Z�b�g
            fallTimer_   = 0.0f;
            isReadyFall_ = false;
        }
    }

    // ��������
    if (isFall_)
    {
        // �������x��ݒ�
        velocity_.y += (fallAcceleration_ * elapsedTime);   
        if (velocity_.y < fallVelocityMax_) velocity_.y = fallVelocityMax_;

        // ����
        transform->AddPositionY(velocity_.y);               

        // ���܂ŗ��������珉���ʒu�ɖ߂��ă��Z�b�g����
        if (transform->GetPosition().y < -20.0f)
        {
            // �p�����[�^���Z�b�g
            transform->SetPositionY(originPosition_.y);
            isFall_      = false;
            velocity_.y  = 0.0f;

            // �_�ł�����
            blinkTimer_  = 0.5f;

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
            blinkTimer_ = 0.0f;
            materialColor = { 0.7f, 0.7f, 0.7f, 1.0f };
        }
        else if (static_cast<int>(blinkTimer_ * 100.0f) & 0x08)
        {
            materialColor = { 1,1,1,1 };
        }
        else
        {
            materialColor = { 0.7f, 0.7f, 0.7f, 1.0f };
        }
    }

    UpdateAABB();   // AABB�̍X�V����
}


void TerrainDelayFall::DrawDebug()
{
    ImGui::Begin(GetName());
    Terrain::DrawDebug();
    ImGui::End();
}


void TerrainDelayFall::OnRiding()
{
    if (isReadyFall_ || isFall_) return;    // ���łɗ����������Ă��邩�A�������Ă�����return

    isReadyFall_ = true;   // ���������t���O�𗧂Ă�
    blinkTimer_  = 0.5f;   // �_�ł�����
}
