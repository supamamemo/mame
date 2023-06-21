#include "CannonBall.h"

#include "../Mame/Graphics/Graphics.h"

#include "CannonBallManager.h"

int CannonBall::nameNum = 0;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="newPosition">大砲の位置</param>
/// <param name="directionZ">砲弾のすすむ方向Z</param>
CannonBall::CannonBall(DirectX::XMFLOAT3 newPosition, float directionZ, CannonBallManager* manager)
    :manager(manager),
    directionZ(directionZ)
{
    manager->Register(this);

    Graphics& graphics = Graphics::Instance();

    // model読み込み
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_cannon_bullet.fbx", true);

    // 初期座標を大砲の位置から少し前にする
    {
        offsetZ = (directionZ > 0) ? offsetZ : -offsetZ;
        newPosition.z += offsetZ;
        GetTransform()->SetPosition(newPosition);
    }

    // imgui名前かぶり起きないように...
    {
        name = "Boss" + std::to_string(nameNum);
        SetName(name.c_str());
        ++nameNum;
    }
}

// デストラクタ
CannonBall::~CannonBall()
{
}

// 初期化
void CannonBall::Initialize()
{
}

// 終了化
void CannonBall::Finalize()
{
}

// Updateの前に呼ばれる
void CannonBall::Begin()
{
}

// 更新処理
void CannonBall::Update(const float& elapsedTime)
{
    float speed = (directionZ > 0) ? moveSpeed * elapsedTime : -moveSpeed * elapsedTime;

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    pos.z += speed;
    GetTransform()->SetPosition(pos);
}

// Updateの後に呼ばれる
void CannonBall::End()
{
}

// 描画処理
void CannonBall::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // Transform更新
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (&keyframe)
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), &keyframe);
    }
    else
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    }

#if _DEBUG
    // BOUNDING_BOX
    {
        DirectX::XMFLOAT4X4 t = SetDebugModelTransform(transform);
        debugModel->skinned_meshes.render(graphics.GetDeviceContext(), t, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.2f), nullptr);
    }
#endif // _DEBUG
}

// imgui
void CannonBall::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    Character::DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}

// 破棄
void CannonBall::Destroy()
{
    manager->Remove(this);
}
