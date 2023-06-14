#include "Boss.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"

int Boss::nameNum = 0;

// コンストラクタ
Boss::Boss()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_block.fbx", true);

    // imgui名前かぶり起きないように...
    name = "Boss" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

Boss::Boss(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), filename, true);

    // imgui名前かぶり起きないように...
    name = "Boss" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// デストラクタ
Boss::~Boss()
{
}

// 初期化
void Boss::Initialize()
{
    model->GetTransform()->SetPosition(DirectX::XMFLOAT3(2, 0, 10));
}

// 終了化
void Boss::Finalize()
{
}

// Updateの前に呼ばれる
void Boss::Begin()
{
}

// 更新処理
void Boss::Update()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = gamePad.GetAxisRX();

    float speed = 0.001f;
    speed *= ax;
    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    pos.x += speed;
    model->GetTransform()->SetPosition(pos);
}

// Updateの後に呼ばれる
void Boss::End()
{
}

// 描画処理
void Boss::Render(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();

    // Transform更新
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (model->skinned_meshes.animation_clips.size() > 0)
    {
        int clip_index{ 0 };
        int frame_index{ 0 };
        static float animation_tick{ 0 };

        animation& animation{ model->skinned_meshes.animation_clips.at(clip_index) };
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

        model->skinned_meshes.render(deviceContext, transform, DirectX::XMFLOAT4(1, 1, 1, 1), &keyframe);
    }
    else
    {
        model->skinned_meshes.render(deviceContext, transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
    }
}

// debug用
void Boss::DrawDebug()
{
    ImGui::Begin(GetName());

    Character::DrawDebug();

    ImGui::End();
}
