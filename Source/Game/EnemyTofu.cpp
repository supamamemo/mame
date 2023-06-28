#include "EnemyTofu.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"

int EnemyTofu::nameNum = 0;

// �R���X�g���N�^
EnemyTofu::EnemyTofu()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/tohuall.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhuwalk.fbx", true);

    // �X�e�[�g�}�V��
    stateMachine.reset(new StateMachine);

    GetStateMachine()->RegisterState(new TOFU::WalkState(this));
    GetStateMachine()->RegisterState(new TOFU::TurnState(this));
    GetStateMachine()->RegisterState(new TOFU::FindState(this));
    GetStateMachine()->RegisterState(new TOFU::TrackState(this));

    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Walk));


    // imgui���O���Ԃ肪�N���Ȃ��悤��...
    name = "EnemyTofu" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// �f�X�g���N�^
EnemyTofu::~EnemyTofu()
{
}

// ������
void EnemyTofu::Initialize()
{
}

// �I����
void EnemyTofu::Finalize()
{
}

// Update�̑O�ɌĂяo�����
void EnemyTofu::Begin()
{
}

// �X�V����
void EnemyTofu::Update(float elapsedTime)
{
    // �X�e�[�g�X�V
    if (stateMachine)GetStateMachine()->Update(elapsedTime);
}

// Update�̌�ɌĂяo�����
void EnemyTofu::End()
{
}

// �`�揈��
void EnemyTofu::Render(float elapsedTime)
{
    Character::Render(elapsedTime);

    //Graphics& graphics = Graphics::Instance();

    //// world�s��X�V
    //NO_CONST DirectX::XMFLOAT4X4 transform = {};
    //DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    //// model�`��
    //if (&model->keyframe)
    //{
    //    model->skinned_meshes.render(graphics.GetDeviceContext(), transform, materialColor, &model->keyframe);
    //}
    //else
    //{
    //    model->skinned_meshes.render(graphics.GetDeviceContext(), transform, materialColor, nullptr);
    //}
}

// debug�p
void EnemyTofu::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    GetTransform()->DrawDebug();

    if (stateMachine)GetStateMachine()->DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}