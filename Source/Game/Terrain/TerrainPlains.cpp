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
    terrainType_ = Terrain::Type::Normal; // 普通の地形タイプに設定
}

void TerrainPlains::Finalize()
{
}

void TerrainPlains::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABBの更新処理
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
