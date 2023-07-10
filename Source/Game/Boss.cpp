#include "Boss.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Graphics/Camera.h"
#include "../Mame/Input/Input.h"
#include "../Mame/AudioManager.h"

#include "BossStateDerived.h"

int Boss::nameNum = 0;

// �R���X�g���N�^
Boss::Boss()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/bossall.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_block.fbx", true);
    DirectX::XMFLOAT3 pos1 = model->GetTransform()->GetPosition();


    // �X�e�[�g�}�V�[�����Z�b�g
    stateMachine.reset(new StateMachine);

    GetStateMachine()->RegisterState(new BOSS::IdleState(this));
    GetStateMachine()->RegisterState(new BOSS::FindState(this));
    GetStateMachine()->RegisterState(new BOSS::TurnState(this));
    GetStateMachine()->RegisterState(new BOSS::AttackState(this));
    GetStateMachine()->RegisterState(new BOSS::RecoilState(this));
    GetStateMachine()->RegisterState(new BOSS::DamageState(this));
    GetStateMachine()->RegisterState(new BOSS::CryState(this));
    GetStateMachine()->RegisterState(new BOSS::WalkState(this));

    GetStateMachine()->SetState(static_cast<int>(BOSS::STATE::Walk));


    // imgui���O���Ԃ�N���Ȃ��悤��...
    name = "Boss" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}


Boss::Boss(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), filename, true);

    // imgui���O���Ԃ�N���Ȃ��悤��...
    name = "Boss" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}


// �f�X�g���N�^
Boss::~Boss()
{
}


// ������
void Boss::Initialize()
{
    GetTransform()->SetPosition(DirectX::XMFLOAT3(12.4f, 2.2f, 10.0f));

    // �������̏�����]�l�ɂ���ĐU������̕������ς��̂Œ���
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, ToRadian(270.0f), 0.0f, 0.0f));

    // TODO: �{�X�̓����蔻��ݒ�
    const DirectX::XMFLOAT3 min = { -0.6f, -0.0f, -0.6f };  // min�ݒ�
    const DirectX::XMFLOAT3 max = { +0.6f, +2.0f, +0.6f };  // max�ݒ�
    SetAABB(min, max);                                      // min��max�̍Đݒ�i�W�I���g���b�N�v���~�e�B�u�̍Đ������s���Ă���j
    UpdateAABB();                                           // min��max�����݂̈ʒu�ɍX�V����

    turnSpeed_ = ToRadian(180.0f);  // ���񑬓x�̐ݒ�

    characterType_ = CharacterType::Enemy;

    // ���G��Ԃɂ���
    isInvincible = true;
}


// �I����
void Boss::Finalize()
{
}


// Update�̑O�ɌĂ΂��
void Boss::Begin()
{
}


// �X�V����
void Boss::Update(const float& elapsedTime)
{
    //GamePad& gamePad = Input::Instance().GetGamePad();

    //float ax = gamePad.GetAxisRX();

    //float speed = 0.001f;
    //speed *= ax;
    //DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    //pos.x += speed;
    //model->GetTransform()->SetPosition(pos);

    if (stateMachine) GetStateMachine()->Update(elapsedTime); // �X�e�[�g�X�V����

    UpdateAABB();                       // AABB�̍X�V����

    UpdateVelocity(elapsedTime);        // ���͏����X�V����

    // �Ђ��ł���Ƃ��ȂǂɃv���C���[�ƏՓ˂��ăv���C���[���_���[�W��H���Ȃ��悤��
    // �Փ˔��菈���̓X�e�[�g���ɍs��
    // CollisionEnemyVsPlayer();

    UpdateInvincibleTimer(elapsedTime); // ���G���Ԃ̍X�V����

    // ���G���Ԓ��̃L�����N�^�[�̓_��(�_���[�W���󂯂����̊m�F�p)
    {
        if (invincibleTimer > 0.0f)
        {
            modelColor.w = (static_cast<int>(invincibleTimer * 100.0f) & 0x04) ? 0.7f : 0.0f;
        }
        else
        {
            modelColor.w = 1.0f;
        }
    }

    UpdateAnimation(elapsedTime);       // �A�j���[�V�����X�V
}

// Update�̌�ɌĂ΂��
void Boss::End()
{
}

// �`�揈��
void Boss::Render(const float& elapsedTime)
{
    Enemy::Render(elapsedTime);
}

// debug�p
void Boss::DrawDebug()
{
    ImGui::Begin(GetName());

    Character::DrawDebug();

    if (stateMachine) GetStateMachine()->DrawDebug();

    ImGui::End();
}


void Boss::OnDead()
{
    stateMachine->ChangeState(static_cast<int>(BOSS::STATE::Damage));
    stateMachine->GetCurrentState()->SetTimer(1.0f);

    // ����������
    //Destroy();
}


void Boss::OnHitWall()
{
    Camera::Instance().PlayShake(ShakeType::HorizontalShake);
    SetMoveDirectionX(-GetMoveDirectionX());    // �ړ������𔽓]
    GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Recoil));  // �����X�e�[�g�֑J��

    
    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopSE(SE::Boss_Run);              // ���sSE��~
    audioManager.PlaySE(SE::Boss_HitWall, false);   // �ǏՓ�SE�Đ�
}

void Boss::OnDamaged()
{
    stateMachine->ChangeState(static_cast<int>(BOSS::STATE::Damage));
}
