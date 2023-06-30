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
    health = 1;                                             // �̗͐ݒ�
  
    moveRangeCenterX_ = GetTransform()->GetPosition().x; // ���݂�X�ʒu���ړ��͈͂̒��S�ɐݒ�

    turnSpeed_ = ToRadian(3.0f);    // ���񑬓x�ݒ�
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
void EnemyTofu::Update(const float& elapsedTime)
{
    // �X�e�[�g�X�V
    if (stateMachine) GetStateMachine()->Update(elapsedTime);

    UpdateAABB();                       // AABB�̍X�V����

    UpdateVelocity(elapsedTime);        // ���͏����X�V����

    CollisionEnemyVsPlayer();           // �v���C���[�Ƃ̏Փ˔��菈��
 
    UpdateInvincibleTimer(elapsedTime); // ���G���ԍX�V
    
    UpdateAnimation(elapsedTime);       // �A�j���[�V�����X�V
}

// Update�̌�ɌĂяo�����
void EnemyTofu::End()
{
}

// �`�揈��
void EnemyTofu::Render(const float& elapsedTime)
{
    Enemy::Render(elapsedTime);
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


void EnemyTofu::OnDead()
{
    // ����������
    Destroy();
}
