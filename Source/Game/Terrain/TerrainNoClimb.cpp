#include "TerrainNoClimb.h"

TerrainNoClimb::TerrainNoClimb(const char* const filename)
    : Terrain(filename)
{
}

TerrainNoClimb::~TerrainNoClimb()
{
}

void TerrainNoClimb::Initialize()
{
    terrainType_ = Terrain::Type::NoClimb; // �o��Ȃ��n�`�^�C�v�ɐݒ�
}

void TerrainNoClimb::Finalize()
{
}

void TerrainNoClimb::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABB�̍X�V����
}

void TerrainNoClimb::Render(const float& elapsedTime)
{
    Terrain::Render(elapsedTime);
}

void TerrainNoClimb::DrawDebug()
{
    ImGui::Begin(GetName());

    Terrain::DrawDebug();

    ImGui::End();
}
