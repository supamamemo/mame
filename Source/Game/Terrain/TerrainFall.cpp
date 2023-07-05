#include "TerrainFall.h"

TerrainFall::TerrainFall(const char* const filename)
    : Terrain(filename)
{
}

void TerrainFall::Initialize()
{
    terrainType_ = Terrain::Type::Move; // 動く地形タイプに設定
}

void TerrainFall::Update(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    // 落下フラグが立っている場合は落下処理を行う
    if (isFall_)
    {
        velocity_.y += (fallAcceleration_ * elapsedTime); // 落下速度を加算
        if (velocity_.y < fallVelocityMax_) velocity_.y = fallVelocityMax_;
        transform->AddPositionY(velocity_.y);       // 落下

        // 一定まで落下したら自分を削除する
        if (transform->GetPosition().y < -10.0f)
        {
            Destroy();  
        }
    }

    UpdateAABB();   // AABBの更新処理
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
    isFall_ = true;     // 落下開始
}
