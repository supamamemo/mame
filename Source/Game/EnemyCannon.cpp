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

    // AABB�X�V����
    UpdateAABB(elapsedTime);

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
    // ���ʂ̕`�揈��
    Character::Render(elapsedTime);

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
