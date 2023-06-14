#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"

// �R���X�g���N�^
StageBoss::StageBoss()
{
    // player����
    player = std::make_unique<Player>();

    // boss����
    boss = std::make_unique<Boss>();


    Graphics& graphics = Graphics::Instance();
    stage[0] = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_ground.fbx", true);
    stage[1] = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_block.fbx", true);
}

// ������
void StageBoss::Initialize()
{
    // player������
    player->Initialize();

    // boss������
    boss->Initialize();
}

// �I����
void StageBoss::Finalize()
{
    // player�I����
    player->Finalize();

    // boss�I����
    boss->Finalize();
}

// Update�̑O�ɌĂ΂��
void StageBoss::Begin()
{
    // player
    player->Begin();

    // boss
    boss->Begin();
}

// �X�V����
void StageBoss::Update(const float& elapsedTime)
{
    // player�X�V
    player->Update(elapsedTime);

    // boss�X�V
    boss->Update();
}

// Update�̌�ɌĂ΂�鏈��
void StageBoss::End()
{
    // player
    player->End();

    // boss
    boss->End();
}

// �`�揈��
void StageBoss::Render(const float& elapsedTime)
{
    // player�`��
    player->Render(elapsedTime);

    // boss�`��
    boss->Render(elapsedTime);

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();

    // Transform�X�V
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, stage[0]->GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��
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

// debug�p
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
