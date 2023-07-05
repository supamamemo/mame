#include "EnemyTofu.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"
#include "PlayerManager.h"

int EnemyTofu::nameNum = 0;

// �R���X�g���N�^
EnemyTofu::EnemyTofu()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/tohuall.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhuwalk.fbx", true);

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
    // �������̏�����]�l�ɂ���ĐU������̕������ς��̂Œ���
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(180), 0, 0));

    health      = 1;                // �̗͐ݒ�
    moveSpeed_  = 1.0f;             // �ړ����x�ݒ�(�X�e�[�g�ŕς��̂ōŏ�����)
    turnSpeed_  = ToRadian(90.0f);  // ���񑬓x�ݒ�(�X�e�[�g�ŕς��̂ōŏ�����)
    jumpSpeed_  = 12.0f;            // �W�����v���x�ݒ�

    // �X�e�[�g�}�V��
    {
        // ���݂�X�ʒu���ړ��͈͂̒��S�ɐݒ�
        moveRangeCenterX_ = GetTransform()->GetPosition().x;

        // �ړ��͈͂̒��S����ړ������Ɉړ��͈͂̔��a���i�񂾈ʒu��ړI�n�ɐݒ�
        destination_ = moveRangeCenterX_ + (moveDirectionX_ * moveRangeRadius_);

        stateMachine.reset(new StateMachine);
        GetStateMachine()->RegisterState(new TOFU::WalkState(this));
        GetStateMachine()->RegisterState(new TOFU::TurnState(this));
        GetStateMachine()->RegisterState(new TOFU::FindState(this));
        GetStateMachine()->RegisterState(new TOFU::TrackState(this));
        GetStateMachine()->RegisterState(new TOFU::IdleBattleState(this));
        GetStateMachine()->RegisterState(new TOFU::DeathState(this));
    }

    // �i�s�����Ɍ������邽�߂ɍŏ��͐���X�e�[�g�ɑJ�ڂ�����
    //GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Walk));
    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Turn));
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

    // �n�`�̒[�𒴂��ė��������ɂȂ�����C��
    {
        if (lastLandingTerrainAABBMaxX != 0.0f && aabb_.max.x > lastLandingTerrainAABBMaxX)
        {
            const float fixLeft = -fabsf(aabb_.max.x - lastLandingTerrainAABBMaxX);
            GetTransform()->AddPositionX(fixLeft);
        }
        else if (lastLandingTerrainAABBMinX != 0.0f && aabb_.min.x < lastLandingTerrainAABBMinX)
        {
            const float fixRight = fabsf(aabb_.min.x - lastLandingTerrainAABBMinX);
            GetTransform()->AddPositionX(fixRight);
        }
    }

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


void EnemyTofu::OnLanding()
{
    // ���n���ɓG�ɏ�������Ă��邩�̃t���O�����Z�b�g����
    isOnFriend_ = false;
}


void EnemyTofu::OnDamaged()
{
}


void EnemyTofu::OnDead()
{
    // �v���C���[�̈ړ������ۑ��x�N�g�����擾
    const float plSaveMoveVecX   = PlayerManager::Instance().GetPlayer()->GetSaveMoveVecX();
    const float length           = fabsf(plSaveMoveVecX);
    const float plSaveMoveVecX_n = (length > 0.0f) ? (plSaveMoveVecX / length) : 1.0f; // �P�ʃx�N�g����(�[���x�N�g���̗\�h����)

    moveDirectionX_ = plSaveMoveVecX_n; // �v���C���[�̍U�������ɐ�����Ԃ悤�ɂ���

    GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Death));
}


void EnemyTofu::OnFallDead()
{
    /// ����������
    Destroy();
}


// �ǂɓ����������ɌĂ΂�鏈��
void EnemyTofu::OnHitWall()
{
    // ���s�X�e�[�g�̂Ƃ��̂ݔ��]�������s��
    const int stateWalk         = static_cast<int>(TOFU::STATE::Walk);
    const int currentStateIndex = GetStateMachine()->GetStateIndex();
    if (currentStateIndex != stateWalk) return;

    moveDirectionX_ = -moveDirectionX_;  // �ړ������𔽓]

    // ����X�e�[�g�֑J��
    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Turn));
}

// �U�������Ƃ��ɌĂ΂�鏈��
void EnemyTofu::OnAttacked()
{
    // �ǐՒ��łȂ����return
    const int stateTrack        = static_cast<int>(TOFU::STATE::Track);
    const int currentStateIndex = GetStateMachine()->GetStateIndex();
    if (currentStateIndex != stateTrack) return;

    // �퓬�ҋ@�X�e�[�g�֑J��
    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::IdleBattle));
}


// �����ɓ����������ɌĂ΂�鏈��
void EnemyTofu::OnHitFriend()
{
    // ���s�X�e�[�g�̂Ƃ��̂ݔ��]�������s��
    const int stateWalk         = static_cast<int>(TOFU::STATE::Walk);
    const int currentStateIndex = GetStateMachine()->GetStateIndex();
    if (currentStateIndex != stateWalk) return;

    moveDirectionX_ = -moveDirectionX_;  // �ړ������𔽓]

    // ����X�e�[�g�֑J��
    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Turn));
}
