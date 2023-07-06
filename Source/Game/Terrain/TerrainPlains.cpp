#include "TerrainPlains.h"

TerrainPlains::TerrainPlains(const char* const filename)
    : Terrain(filename)
{
}

TerrainPlains::~TerrainPlains()
{
}

void TerrainPlains::Initialize()
{
    terrainType_ = Terrain::Type::Normal; // ���ʂ̒n�`�^�C�v�ɐݒ�
}

void TerrainPlains::Finalize()
{
}

void TerrainPlains::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABB�̍X�V����
}

void TerrainPlains::Render(const float& elapsedTime)
{
    Terrain::Render(elapsedTime);
}

void TerrainPlains::DrawDebug()
{
    ImGui::Begin(GetName());

    Terrain::DrawDebug();

    ImGui::End();
}
