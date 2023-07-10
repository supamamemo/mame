#include "TerrainDelayFall.h"

#include "../PlayerManager.h"

TerrainDelayFall::TerrainDelayFall(const char* const filename)
    : Terrain(filename)
{
}

void TerrainDelayFall::Initialize()
{
    Terrain::Initialize();

    terrainType_ = Terrain::Type::Move; // 動く地形タイプに設定
}

void TerrainDelayFall::Update(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    // 落下準備フラグが立っている場合は落下タイマーを起動する
    if (!isFall_ && isReadyFall_)
    {
        fallTimer_ += elapsedTime;

        // 一定時間たったら落下フラグを立てる
        if (fallTimer_ >= 0.5f)
        {
            isFall_      = true;

            // パラメータリセット
            fallTimer_   = 0.0f;
            isReadyFall_ = false;
        }
    }

    // 落下処理
    if (isFall_)
    {
        // 落下速度を設定
        velocity_.y += (fallAcceleration_ * elapsedTime);   
        if (velocity_.y < fallVelocityMax_) velocity_.y = fallVelocityMax_;

        // 落下
        transform->AddPositionY(velocity_.y);               

        // 一定まで落下したら初期位置に戻ってリセットする
        if (transform->GetPosition().y < -20.0f)
        {
            // パラメータリセット
            transform->SetPositionY(originPosition_.y);
            isFall_      = false;
            velocity_.y  = 0.0f;

            // 点滅させる
            blinkTimer_  = 0.5f;

            // プレイヤーが保存している地形情報の地形が自分だったらその地形情報を消去
            // (リセット時にプレイヤーがついてこないようにするため)
            PlayerManager& playerManager = PlayerManager::Instance();
            if (this == playerManager.GetPlayer()->saveTerrain_)
            {
                playerManager.GetPlayer()->saveTerrain_ = nullptr;
            }
        }
    }
    
    // 点滅タイマーが設定されていれば点滅させる
    if (blinkTimer_ > 0.0f)
    {
        blinkTimer_ -= elapsedTime;

        if (blinkTimer_ <= 0.0f)
        {
            blinkTimer_ = 0.0f;
            materialColor = { 1,1,1,1 };
        }
        else if (static_cast<int>(blinkTimer_ * 100.0f) & 0x08)
        {
            materialColor = { 1.5f, 1.5f, 1.5f, 1.0f };
        }
        else
        {
            materialColor = { 1,1,1,1 };
        }
    }

    UpdateAABB();   // AABBの更新処理
}


void TerrainDelayFall::DrawDebug()
{
    ImGui::Begin(GetName());
    Terrain::DrawDebug();
    ImGui::End();
}


void TerrainDelayFall::OnRiding()
{
    isReadyFall_ = true;   // 落下準備フラグを立てる
    blinkTimer_  = 0.5f;   // 点滅させる
}
