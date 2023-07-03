#include "Box.h"

#include "../Mame/Graphics/Graphics.h"

#include "PlayerManager.h"

int Box::nameNum = 0;
float Box::renderLengthXLimit_ = 40.0f;

// コンストラクタ
Box::Box(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), filename, true);

    // imgui名前かぶり起きないように...
    name = "Box" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// 初期化
void Box::Initialize()
{
}

// 終了化
void Box::Finalize()
{
}

// 更新処理
void Box::Update(const float& elapsedTime)
{
}

// 描画処理
void Box::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // DrawCollを少なくするためにplayerから近いものだけ描画する
    {
        const float& boxPosX    = GetTransform()->GetPosition().x;
        const float& playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float  lengthX    = fabsf(boxPosX - playerPosX);

        if (lengthX > renderLengthXLimit_) return;
    }

    // world行列更新
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (&model->keyframe)
    {
        model->skinned_meshes->render(graphics.GetDeviceContext(), transform, GetMaterialColor(), &model->keyframe);
    }
    else
    {
        model->skinned_meshes->render(graphics.GetDeviceContext(), transform, GetMaterialColor(), nullptr);
    }
}

// debug用
void Box::DrawDebug()
{
#ifdef USE_IMGUI
    ImGui::Begin(GetName());

    GetTransform()->DrawDebug();

    ImGui::ColorEdit4("color", &materialColor.x);

    ImGui::End();
#endif
}
