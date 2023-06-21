#include "EnemyCannon.h"

#include "../Mame/Graphics/Graphics.h"

int EnemyCannon::nameNum = 0;

// コンストラクタ
EnemyCannon::EnemyCannon()
{
    Graphics& graphics = Graphics::Instance();

    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhu.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_cannon.fbx", true);

    debugModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/cube.fbx", true);

    // imgui名前かぶりが起きないように...
    name = "EnemyCannon" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// デストラクタ
EnemyCannon::~EnemyCannon()
{
}

// 初期化
void EnemyCannon::Initialize()
{
}

// 終了化
void EnemyCannon::Finalize()
{
}

// Updateの前に呼ばれる
void EnemyCannon::Begin()
{
}

// 更新処理
void EnemyCannon::Update(float elapsedTime)
{

}

// Updateの後に呼ばれる
void EnemyCannon::End()
{
}

// 描画処理
void EnemyCannon::Render(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // Transform更新
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (model->skinned_meshes.animation_clips.size() > 0)
    {
        int clipIndex{ GetAnimationIndex() };
        int frameIndex{ 0 };
        static float animationTick{ 0 };

        animation& animation{ model->skinned_meshes.animation_clips.at(clipIndex) };
        frameIndex = static_cast<int>(animationTick * animation.sampling_rate);
        if (frameIndex > animation.sequence.size() - 1)
        {
            frameIndex = 0;
            animationTick = 0;
        }
        else
        {
            animationTick += elapsedTime;
        }
        animation::keyframe& keyframe{ animation.sequence.at(frameIndex) };

        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, 1), &keyframe);
    }
    else
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
    }

#if _DEBUG
    // BOUNDING_BOX
    {
        DirectX::XMFLOAT4X4 t = SetDebugModelTransform(transform);
        debugModel->skinned_meshes.render(graphics.GetDeviceContext(), t, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.2f), nullptr);
    }
#endif // _DEBUG
}

// debug用
void EnemyCannon::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    model->GetTransform()->DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}
