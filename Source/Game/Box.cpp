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
    Render(elapsedTime, 0.01f);
}

// 描画処理
void Box::Render(const float& elapsedTime, float size)
{
    Graphics& graphics = Graphics::Instance();

    // DrawCollを少なくするためにplayerから近いものだけ描画する
    {
        const float& boxPosX = GetTransform()->GetPosition().x;
        const float& playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float  lengthX = fabsf(boxPosX - playerPosX);

        if (lengthX > renderLengthXLimit_) return;
    }

    // world行列更新
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(size));

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

void Box::Render(const float& elapsedTime, bool noPlayer, float size)
{
    Graphics& graphics = Graphics::Instance(); 

    // world行列更新
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(size));

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

void Box::SelectBossInitialize()
{
    model->PlayAnimation(4, true);
}

void Box::SelectBossUpdate(float elapsedTime)
{
    model->UpdateAnimation(elapsedTime);
}

void Box::BackUpdate(float elapsedTime)
{
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    DirectX::XMFLOAT3 cameraPos = Camera::Instance().GetTransform()->GetPosition();

    pos.x = cameraPos.x;

    GetTransform()->SetPosition(pos);
}
