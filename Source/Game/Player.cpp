#include "Player.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"

Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    
    //model = new Model(graphics.GetDevice(), "./resources/idle3.fbx", false);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/idletest.fbx", true);
    //model = new Model(graphics.GetDevice(), "./resources/idletest.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/nopark.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/hiyokomame.fbx", true);
    //model = new Model(graphics.GetDevice(), "./resources/hiyokomame.fbx", true);    //model = new Model(graphics.GetDevice(), "./resources/mame.fbx", 0, true);
    //model = new Model(graphics.GetDevice(), "./resources/byoga/plantune.fbx", 0, true);
    //model = new Model(graphics.GetDevice(), "/resources/byoga/nico.fbx");
}

Player::~Player()
{
}

void Player::Initialize()
{
    model->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 10));
    model->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(180), 0, 0));
    model->GetTransform()->SetScale(DirectX::XMFLOAT3(3, 3, 3));
}

void Player::Finalize()
{
}

void Player::Begin()
{
}

void Player::Update()
{
    // GamePad取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();

    if (gamePad.GetButton() & GamePad::BTN_B)
    {
        pos.x += 0.01f;
    }

    model->GetTransform()->SetPosition(pos);

    InputMove();
}

void Player::End()
{
}

void Player::Render(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

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

        model->skinned_meshes.render(immediate_context, transform, DirectX::XMFLOAT4(1, 1, 1, 1), &keyframe);
    }
    else
    {
        model->skinned_meshes.render(immediate_context, transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
    }
}

void Player::DrawDebug()
{
    Character::DrawDebug();
}

// 入力移動
void Player::InputMove()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    
    // これはキーボード限定？？
#if 0
    if (gamePad.GetButton() & GamePad::BTN_RIGHT)pos.x += 0.001f;
    if (gamePad.GetButton() & GamePad::BTN_LEFT)pos.x -= 0.001f;
#else
    float ax = gamePad.GetAxisLX();
    //if (ax == 1)pos.x += 0.001f;
    //if (ax == -1)pos.x -= 0.001f;
#endif

    model->GetTransform()->SetPosition(pos);

}


