#include "EnemyCannon.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"

int EnemyCannon::nameNum = 0;

// �R���X�g���N�^
EnemyCannon::EnemyCannon()
{
    Graphics& graphics = Graphics::Instance();

    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhu.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_cannon.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_flag.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/matome0622.fbx", true);

    // �X�e�[�g�}�V�����Z�b�g
    stateMachine.reset(new StateMachine);

    GetStateMachine()->RegisterState(new CANNON::IdleState(this));
    GetStateMachine()->RegisterState(new CANNON::AttackState(this));

    GetStateMachine()->SetState(static_cast<int>(CANNON::STATE::Idle));


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
    // cannonBallManager
    cannonBallManager.Update(elapsedTime);

    if (stateMachine) GetStateMachine()->Update(elapsedTime);

    // �f�o�b�O���f���̈ʒu�X�V
    debugModel->GetTransform()->SetPosition(model->GetTransform()->GetPosition());
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
    if (&model->keyframe)
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, materialColor, &model->keyframe);
    }
    else
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
    }

#if _DEBUG
    // BOUNDING_BOX
    {
        DirectX::XMFLOAT4X4 debugTransform = {};

        // ���[���h�s��̎擾�ƃX�P�[������
        DirectX::XMStoreFloat4x4(&debugTransform, debugModel->GetTransform()->CalcWorldMatrix(0.01f));

        // ���[���h�s��ݒ�
        debugTransform = SetDebugModelTransform(debugTransform);

        // �`��
        debugModel->skinned_meshes.render(graphics.GetDeviceContext(), debugTransform, { 1.0f, 0.0f, 0.0f, 0.2f }, nullptr);
    }
#endif // _DEBUG

    // cannonBallManager
    cannonBallManager.Render(elapsedTime);

}

// debug�p
void EnemyCannon::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    model->GetTransform()->DrawDebug();

    if (stateMachine) GetStateMachine()->DrawDebug();

    cannonBallManager.DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}
