#include "TerrainNoCollision.h"

void TerrainNoCollision::Initialize()
{
    Terrain::Initialize();
    terrainType_ = Terrain::Type::NoCollision;  // �����蔻��Ȃ��^�C�v�̒n�`�ɐݒ�
}

void TerrainNoCollision::DrawDebug()
{
    ImGui::Begin(GetName());
    Terrain::DrawDebug();
    ImGui::End();
}
