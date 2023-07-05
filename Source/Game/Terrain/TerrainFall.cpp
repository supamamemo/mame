#include "TerrainFall.h"

TerrainFall::TerrainFall(const char* const filename)
    : Terrain(filename)
{
}

void TerrainFall::Initialize()
{
    terrainType_ = Terrain::Type::Move; // �����n�`�^�C�v�ɐݒ�
}

void TerrainFall::Update(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    // �����t���O�������Ă���ꍇ�͗����������s��
    if (isFall_)
    {
        velocity_.y += (fallAcceleration_ * elapsedTime); // �������x�����Z
        if (velocity_.y < fallVelocityMax_) velocity_.y = fallVelocityMax_;
        transform->AddPositionY(velocity_.y);       // ����

        // ���܂ŗ��������玩�����폜����
        if (transform->GetPosition().y < -10.0f)
        {
            Destroy();  
        }
    }

    UpdateAABB();   // AABB�̍X�V����
}

void TerrainFall::Render(const float& elapsedTime)
{
    Terrain::Render(elapsedTime);
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
