#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"

// コンストラクタ
StageBoss::StageBoss()
{
    // player生成
    player = std::make_unique<Player>();

    // boss生成
    boss = std::make_unique<Boss>();


    Graphics& graphics = Graphics::Instance();
    stage[0] = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_ground.fbx", true);
    stage[1] = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_block.fbx", true);
}

// 初期化
void StageBoss::Initialize()
{
    // player初期化
    player->Initialize();

    // boss初期化
    boss->Initialize();
}

// 終了化
void StageBoss::Finalize()
{
    // player終了化
    player->Finalize();

    // boss終了化
    boss->Finalize();
}

// Updateの前に呼ばれる
void StageBoss::Begin()
{
    // player
    player->Begin();

    // boss
    boss->Begin();
}

// 更新処理
void StageBoss::Update(const float& elapsedTime)
{
    // player更新
    player->Update(elapsedTime);

    // boss更新
    boss->Update();
}

// Updateの後に呼ばれる処理
void StageBoss::End()
{
    // player
    player->End();

    // boss
    boss->End();
}

// 描画処理
void StageBoss::Render(const float& elapsedTime)
{
    // player描画
    player->Render(elapsedTime);

    // boss描画
    boss->Render(elapsedTime);

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();

    // Transform更新
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, stage[0]->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (stage[0]->skinned_meshes.animation_clips.size() > 0)
    {
        int clip_index{ 0 };
        int frame_index{ 0 };
        static float animation_tick{ 0 };

        animation& animation{ stage[0]->skinned_meshes.animation_clips.at(clip_index) };
        frame_index = static_cast<int>(animation_tick * animation.sampling_rate);
        if (frame_index > animation.sequence.size() - 1)
        {
            frame_index = 0;
            animation_tick = 0;
        }
        else
        {
            animation_tick += elapsedTime;
        }
        animation::keyframe& keyframe{ animation.sequence.at(frame_index) };

        stage[0]->skinned_meshes.render(deviceContext, transform, DirectX::XMFLOAT4(1, 1, 1, 1), &keyframe);
    }
    else
    {
        stage[0]->skinned_meshes.render(deviceContext, transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
    }
}

// debug用
void StageBoss::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    player->DrawDebug();

    // boss
    boss->DrawDebug();

    stage[0]->GetTransform()->DrawDebug();

#endif
}
