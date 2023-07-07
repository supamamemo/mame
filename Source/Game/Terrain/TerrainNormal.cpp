#include "TerrainNormal.h"

TerrainNormal::TerrainNormal(const char* const filename)
    : Terrain(filename)
{
}

TerrainNormal::~TerrainNormal()
{
}

void TerrainNormal::Initialize()
{
    terrainType_ = Terrain::Type::Normal; // ���ʂ̒n�`�^�C�v�ɐݒ�
}

void TerrainNormal::Finalize()
{
}

void TerrainNormal::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABB�̍X�V����
}

void TerrainNormal::Render(const float& elapsedTime)
{
    Terrain::Render(elapsedTime);
}

void TerrainNormal::DrawDebug()
{
    ImGui::Begin(GetName());

    Terrain::DrawDebug();

    ImGui::End();
}
