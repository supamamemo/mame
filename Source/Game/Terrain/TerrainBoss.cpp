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
}

void TerrainBoss::Finalize()
{
}

void TerrainBoss::Begin()
{
}

void TerrainBoss::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABB‚ÌXVˆ—
}

void TerrainBoss::End()
{
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
