#include "GrassBlock.h"

#include "../Mame/Graphics/Graphics.h"

#include <stdio.h>

int GrassBlock::nameNum = 0;

// コンストラクタ
GrassBlock::GrassBlock()
{
    Graphics& graphics = Graphics::Instance();

    // model生成
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);


    // 名前生成仮で作った。。。
    auto a = std::to_string(nameNum);

    std::string b = "GrassBlock";
    std::string c = b + a;

    SetName(c.c_str());
    ++nameNum;
}

// デストラクタ
GrassBlock::~GrassBlock()
{
}

// 初期化
void GrassBlock::Initialize(DirectX::XMFLOAT3 pos)
{
    model->GetTransform()->SetPosition(pos);
}

// 終了化
void GrassBlock::Finalize()
{
}

// Updateの前に呼ばれる
void GrassBlock::Begin()
{
}

// 更新処理
void GrassBlock::Update()
{

}

// Updateの後に呼ばれる
void GrassBlock::End()
{
}

// 描画処理
void GrassBlock::Render(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();

    // Transformの更新
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画(アニメーションなし想定)
    model->skinned_meshes.render(deviceContext, transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
}

//debug用
void GrassBlock::DrawDebug()
{
    ImGui::Begin(GetName());
    
    Block::DrawDebug();

    ImGui::End();
}