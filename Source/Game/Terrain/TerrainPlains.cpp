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
}

void TerrainPlains::Finalize()
{
}

void TerrainPlains::Begin()
{
}

void TerrainPlains::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABB‚ÌXVˆ—
}

void TerrainPlains::End()
{
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
