#include "TerrainMoveDown.h"
#include "../PlayerManager.h"

void TerrainMoveDown::Initialize()
{
    Terrain::Initialize();

    terrainType_ = Terrain::Type::Move; // �����n�`�^�C�v�ɐݒ�
}

void TerrainMoveDown::Update(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    // ���~������
    {
        velocity_.y = (addSpeedY_ * elapsedTime);
        transform->AddPositionY(velocity_.y);

        // ����艺�ɍs������ʒu�����Z�b�g
        if (transform->GetPosition().y < positionLimitY_)
        {
            transform->SetPositionY(resetPositionY_);

            // �v���C���[���ۑ����Ă���n�`���̒n�`�������������炻�̒n�`��������
            // (���Z�b�g���Ƀv���C���[�����Ă��Ȃ��悤�ɂ��邽��)
            PlayerManager& playerManager = PlayerManager::Instance();
            if (this == playerManager.GetPlayer()->saveTerrain_)
            {
                playerManager.GetPlayer()->saveTerrain_ = nullptr;
            }
        }
    }

    UpdateAABB();   // AABB�̍X�V����
}

void TerrainMoveDown::DrawDebug()
{
}
