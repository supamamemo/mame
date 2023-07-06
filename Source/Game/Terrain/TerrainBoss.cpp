#include "TerrainBoss.h"

TerrainBoss::TerrainBoss(const char* const filename)
    : Terrain(filename)
{
}

TerrainBoss::~TerrainBoss()
{
}

void TerrainBoss::Initialize()
{
    terrainType_ = Terrain::Type::Normal; // ���ʂ̒n�`�^�C�v�ɐݒ�
}

void TerrainBoss::Finalize()
{
}

void TerrainBoss::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABB�̍X�V����
}

void TerrainBoss::Render(const float& elapsedTime)
{
    Terrain::Render(elapsedTime);
}

void TerrainBoss::DrawDebug()
{
    ImGui::Begin(GetName());

    Terrain::DrawDebug();

    ImGui::End();
}
