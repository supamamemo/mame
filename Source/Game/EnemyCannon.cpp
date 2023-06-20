#include "EnemyCannon.h"

#include "../Mame/Graphics/Graphics.h"

int EnemyCannon::nameNum = 0;

// �R���X�g���N�^
EnemyCannon::EnemyCannon()
{
    Graphics& graphics = Graphics::Instance();

    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhu.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_cannon.fbx", true);

    debugModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/cube.fbx", true);

    // imgui���O���Ԃ肪�N���Ȃ��悤��...
    name = "EnemyCannon" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// �f�X�g���N�^
EnemyCannon::~EnemyCannon()
{
}

// ������
void EnemyCannon::Initialize()
{
}

// �I����
void EnemyCannon::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void EnemyCannon::Begin()
{
}

// �X�V����
void EnemyCannon::Update(float elapsedTime)
{

}

// Update�̌�ɌĂ΂��
void EnemyCannon::End()
{
}

// �`�揈��
void EnemyCannon::Render(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // Transform�X�V
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��
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

// debug�p
void EnemyCannon::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    model->GetTransform()->DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}
