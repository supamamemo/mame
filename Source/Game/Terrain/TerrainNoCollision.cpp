#include "TerrainNoCollision.h"

void TerrainNoCollision::Initialize()
{
    Terrain::Initialize();
    terrainType_ = Terrain::Type::NoCollision;  // 当たり判定なしタイプの地形に設定
}

void TerrainNoCollision::DrawDebug()
{
    ImGui::Begin(GetName());
    Terrain::DrawDebug();
    ImGui::End();
}
