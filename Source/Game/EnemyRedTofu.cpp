#include "EnemyRedTofu.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"
#include "PlayerManager.h"
#include "EnemyTofu.h"

namespace RED_TOFU
{
    //int EnemyRedTofu::nameNum = 0;

    // �R���X�g���N�^
    EnemyRedTofu::EnemyRedTofu()
    {
        Graphics& graphics = Graphics::Instance();

        model = std::make_unique<Model>(graphics.GetDevice(), "./resources/tohuall.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhuwalk.fbx", true);

        // imgui���O���Ԃ肪�N���Ȃ��悤��...
        name = "EnemyTofu" + std::to_string(EnemyTofu::nameNum);    // ��������naneNum���g��
        SetName(name.c_str());
        ++EnemyTofu::nameNum;
    }

    // �f�X�g���N�^
    EnemyRedTofu::~EnemyRedTofu()
    {
    }

    // ������
    void EnemyRedTofu::Initialize()
    {
        // �������̏�����]�l�ɂ���ĐU������̕������ς��̂Œ���
        GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(180), 0, 0));

        modelColor = DirectX::XMFLOAT4(1.0f, 0.25f, 0.25f, 1.0f); // �ԐF�ɐݒ�

        health      = 1;                // �̗͐ݒ�
        moveSpeed_  = 2.0f;             // �ړ����x�ݒ�(�X�e�[�g�ŕς��̂ōŏ�����)
        turnSpeed_  = ToRadian(180.0f); // ���񑬓x�ݒ�(�X�e�[�g�ŕς��̂ōŏ�����)
        jumpSpeed_  = 12.0f;            // �W�����v���x�ݒ�

        moveRangeRadius_ = 8.0f;        // �ړ��͈͂̔��a
        searchLengthX_   = 6.0f;        // X���ō��G�ł��鋗��
        searchLengthY_   = 6.0f;        // Y���ō��G�ł��鋗��

        characterType_ = CharacterType::Enemy;
        enemyType_ = EnemyType::RedTofu;    // �G�̎�ނ�ݒ�

        // �X�e�[�g�}�V��
        {
            // ���݂�X�ʒu���ړ��͈͂̒��S�ɐݒ�
            moveRangeCenterX_ = GetTransform()->GetPosition().x;

            // �ړ��͈͂̒��S����ړ������Ɉړ��͈͂̔��a���i�񂾈ʒu��ړI�n�ɐݒ�
            destination_ = moveRangeCenterX_ + (moveDirectionX_ * moveRangeRadius_);

            stateMachine.reset(new StateMachine);
            GetStateMachine()->RegisterState(new WalkState(this));
            GetStateMachine()->RegisterState(new TurnState(this));
            GetStateMachine()->RegisterState(new FindState(this));
            GetStateMachine()->RegisterState(new TrackState(this));
            GetStateMachine()->RegisterState(new IdleBattleState(this));
            GetStateMachine()->RegisterState(new DeathState(this));
        }

        // �i�s�����Ɍ������邽�߂ɍŏ��͐���X�e�[�g�ɑJ�ڂ�����
        //GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Walk));
        GetStateMachine()->SetState(static_cast<int>(STATE::Turn));
    }

    // �I����
    void EnemyRedTofu::Finalize()
    {
    }

    // Update�̑O�ɌĂяo�����
    void EnemyRedTofu::Begin()
    {
    }

    // �X�V����
    void EnemyRedTofu::Update(const float& elapsedTime)
    {
        // �X�e�[�g�X�V
        if (stateMachine) GetStateMachine()->Update(elapsedTime);

        //// �n�`�̒[�𒴂��ė��������ɂȂ�����C��
        //{
        //    if (lastLandingTerrainAABBMaxX != 0.0f && aabb_.max.x > lastLandingTerrainAABBMaxX)
        //    {
        //        const float fixLeft = -fabsf(aabb_.max.x - lastLandingTerrainAABBMaxX);
        //        GetTransform()->AddPositionX(fixLeft);
        //    }
        //    else if (lastLandingTerrainAABBMinX != 0.0f && aabb_.min.x < lastLandingTerrainAABBMinX)
        //    {
        //        const float fixRight = fabsf(aabb_.min.x - lastLandingTerrainAABBMinX);
        //        GetTransform()->AddPositionX(fixRight);
        //    }
        //}

        UpdateAABB();                       // AABB�̍X�V����

        UpdateVelocity(elapsedTime);        // ���͏����X�V����

        CollisionEnemyVsPlayer();           // �v���C���[�Ƃ̏Փ˔��菈��

        UpdateInvincibleTimer(elapsedTime); // ���G���ԍX�V

        UpdateAnimation(elapsedTime);       // �A�j���[�V�����X�V
    }

    // Update�̌�ɌĂяo�����
    void EnemyRedTofu::End()
    {
    }

    // �`�揈��
    void EnemyRedTofu::Render(const float& elapsedTime)
    {
        Enemy::Render(elapsedTime);
    }

    // debug�p
    void EnemyRedTofu::DrawDebug()
    {
#if USE_IMGUI
        ImGui::Begin(GetName());

        GetTransform()->DrawDebug();

        if (stateMachine)GetStateMachine()->DrawDebug();

        ImGui::End();
#endif // USE_IMGUI
    }


    void EnemyRedTofu::OnLanding()
    {
        // ���n���ɓG�ɏ�������Ă��邩�̃t���O�����Z�b�g����
        isOnFriend_ = false;
    }


    void EnemyRedTofu::OnDamaged()
    {
    }


    void EnemyRedTofu::OnDead()
    {
        // �v���C���[�̈ړ������ۑ��x�N�g�����擾
        const float plSaveMoveVecX = PlayerManager::Instance().GetPlayer()->GetSaveMoveVecX();
        const float length = fabsf(plSaveMoveVecX);
        const float plSaveMoveVecX_n = (length > 0.0f) ? (plSaveMoveVecX / length) : 1.0f; // �P�ʃx�N�g����(�[���x�N�g���̗\�h����)

        moveDirectionX_ = plSaveMoveVecX_n; // �v���C���[�̍U�������ɐ�����Ԃ悤�ɂ���

        GetStateMachine()->ChangeState(static_cast<int>(STATE::Death));
    }


    void EnemyRedTofu::OnFallDead()
    {
        /// ����������
        Destroy();
    }


    // �ǂɓ����������ɌĂ΂�鏈��
    void EnemyRedTofu::OnHitWall()
    {
        const int stateWalk         = static_cast<int>(STATE::Walk);
        const int stateTrack        = static_cast<int>(STATE::Track);
        const int currentStateIndex = GetStateMachine()->GetStateIndex();

        // ���s�X�e�[�g�̂Ƃ��͐���X�e�[�g�֑J��
        if (currentStateIndex == stateWalk)
        {
            moveDirectionX_ = -moveDirectionX_;  // �ړ������𔽓]

            // ����X�e�[�g�֑J��
            GetStateMachine()->SetState(static_cast<int>(STATE::Turn));
            return;
        }
#if 0 // �ǂ����������щz���悤�Ƃ���
        // �ǐՃX�e�[�g�̂Ƃ��͒n�ʂɂ��Ă�����W�����v������
        else if (currentStateIndex == stateTrack && isGround_)
        {         
            Jump(jumpSpeed_ * 1.5f);
            return;
        }        
#else // �ǂ��悶�o��
        // �ǐՃX�e�[�g�̂Ƃ��͕ǂ��悶�o��(�������E�o��Ȃ��^�C�v�̒n�`�Ȃ�o��Ȃ�)
        else if (currentStateIndex == stateTrack && velocity.y >= 0.0f &&
            saveWall_->terrainType_ != Terrain::Type::NoClimb)
        {         
            Jump(jumpSpeed_ * 0.5f);
            return;
        }
#endif
    }

    // �U�������Ƃ��ɌĂ΂�鏈��
    void EnemyRedTofu::OnAttacked()
    {
        // �ǐՒ��łȂ����return
        const int stateTrack = static_cast<int>(STATE::Track);
        const int currentStateIndex = GetStateMachine()->GetStateIndex();
        if (currentStateIndex != stateTrack) return;

        // �퓬�ҋ@�X�e�[�g�֑J��
        GetStateMachine()->SetState(static_cast<int>(STATE::IdleBattle));
    }


    // �����ɓ����������ɌĂ΂�鏈��
    void EnemyRedTofu::OnHitFriend()
    {
        // ���s�X�e�[�g�̂Ƃ��̂ݔ��]�������s��
        const int stateWalk = static_cast<int>(STATE::Walk);
        const int currentStateIndex = GetStateMachine()->GetStateIndex();
        if (currentStateIndex != stateWalk) return;

        moveDirectionX_ = -moveDirectionX_;  // �ړ������𔽓]

        // ����X�e�[�g�֑J��
        GetStateMachine()->SetState(static_cast<int>(STATE::Turn));
    }

}