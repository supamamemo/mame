#include "Player.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"
#include "OperatorXMFLOAT3.h"
#include "EnemyManager.h"
#include "CannonBallManager.h"
#include "BossStateDerived.h"

Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // ���f������
    {
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/enemy_001Ver10.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/castel.fbx", true);

        model = std::make_unique<Model>(graphics.GetDevice(), "./resources/mameoall.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/matome0622.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/jump.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/idletest.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/hiyokomame.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temp.fbx", true);
    }
    
    // create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/wireframe.cso", pixel_shaders.GetAddressOf());

    //sprite = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/load.png");

    //create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/wireframe.cso", pixel_shaders.GetAddressOf());

    const DirectX::XMFLOAT3 min = { -0.4f, -0.0f, -0.4f }; // �����~�X�ɒ���
    const DirectX::XMFLOAT3 max = { +0.4f, +1.0f, +0.4f };
    ResetAABB(min, max);
    UpdateAABB();

    // �ҋ@�X�e�[�g�֑J��
    TransitionIdleState();
}


Player::~Player()
{
}


void Player::Initialize()
{
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(180), 0, 0));
    GetTransform()->SetScale(DirectX::XMFLOAT3(1, 1, 1));
}


void Player::Finalize()
{
}


void Player::Begin()
{
    
}


void Player::Update(const float& elapsedTime)
{  
    // �X�e�[�g���򏈗�
    switch (state)
    {
    case State::Idle:    UpdateIdleState(elapsedTime);    break;
    case State::Walk:    UpdateWalkState(elapsedTime);    break;
    case State::Dash:    UpdateDashState(elapsedTime);    break;
    case State::Run:     UpdateRunState(elapsedTime);     break;
    case State::Jump:    UpdateJumpState(elapsedTime);    break;
    case State::HipDrop: UpdateHipDropState(elapsedTime); break;
    }

    // AABB�X�V����
    UpdateAABB();

    // ���͍X�V����
    UpdateVelocity(elapsedTime);

    // �v���C���[�ƓG�̏Փ˔���
    CollisionPlayerVsEnemies();


    // ���G���Ԓ��̃L�����N�^�[�̓_��
    if (invincibleTimer > 0.0f)
    {
        modelColorAlpha = (static_cast<int>(invincibleTimer * 100.0f) & 0x08) ? 0.7f : 0.0f;
    }
    else
    {
        modelColorAlpha = 1.0f;
    }

    // ���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    // �_�b�V���N�[���^�C���X�V
    UpdateDashCoolTimer(elapsedTime);

    // �A�j���[�V�����X�V
    UpdateAnimation(elapsedTime);
}


void Player::End()
{
}


void Player::Render(const float& elapsedTime)
{
    // ���ʂ̕`�揈��
    Character::Render(elapsedTime);
}


// �f�o�b�O�`��
void Player::DrawDebug()
{
    ImGui::Begin("player");

    Character::DrawDebug();

    ImGui::Checkbox("isDebugBlendAnimation", &model->isDebugBlendAnimation);

    NO_CONST int animationIndex = model->GetCurrentAnimationIndex();
    ImGui::SliderInt("animationIndex", &animationIndex, 0, Anim_Max - 1);
    model->SetCurrentAnimationIndex(animationIndex);

    //ImGui::DragFloat2("box2dLenght", &box2d.lenght.x);

    // ���x�֘A�p�����[�^
    if (ImGui::TreeNode("Speed"))
    {
        ImGui::InputFloat3("velocity",         &velocity.x);                      // ���x(�m�F�p)

        ImGui::SliderFloat("acceleration",     &acceleration,     0.0f, 5.0f);    // ������
        ImGui::SliderFloat("d_Gravity",        &defaultGravity,   0.0f, 5.0f);    // �d��(�o�E���X��ɔ��f)
        ImGui::SliderFloat("hipDropGravity",   &hipDropGravity,   1.0f, -10.0f);  // �q�b�v�h���b�v���̏d��
        ImGui::SliderFloat("friction",         &friction,         0.0f, 5.0f);    // ���C�́i��~����́j
        ImGui::SliderFloat("airControl",       &airControl,       0.0f, 1.0f);    // �󒆂ł̐g�����̂��₷��

        ImGui::SliderFloat("dashAcceleration", &dashAcceleration, 0.0f, 60.0f);   // �_�b�V�����̉�����
        ImGui::SliderFloat("d_MoveSpeed",      &defaultMoveSpeed, 0.0f, 20.0f);   // �ʏ펞�̍ō��ړ����x
        ImGui::SliderFloat("runMoveSpeed",     &runMoveSpeed,     0.0f, 30.0f);   // ���s���̍ō��ړ����x

        ImGui::SliderFloat("turnSpeed_",       &turnSpeed_, 1.0f, ToRadian(900.0f), "%.0f");  // ���񑬓x

        ImGui::TreePop();
    }

    // �W�����v�֘A�p�����[�^
    if (ImGui::TreeNode("Jump"))
    {
        ImGui::SliderFloat("jumpSpeed",  &jumpSpeed,       0.0f, 30.0f);   // �W�����v���x
        ImGui::SliderFloat("d_JumpTime", &defaultJumpTime, 0.0f, 1.0f);    // �W�����v���������鎞��
        ImGui::SliderInt("jumpLimit",    &jumpLimit,       1,    5);       // �W�����v�ő��

        ImGui::TreePop();
    }

    // �o�E���X�֘A�p�����[�^
    if (ImGui::TreeNode("Bounce"))
    {
        ImGui::SliderFloat("d_BounceSpeedX", &defaultBounceSpeedX, 0.0f,  30.0f, "%.0f");   // ���˂�Ƃ���X���x(�����q�b�v�h���b�v���Ă���ύX�����f�����)
        ImGui::SliderFloat("d_BounceSpeedY", &defaultBounceSpeedY, 0.0f,  30.0f, "%.0f");   // ���˂�Ƃ���Y���x(�����q�b�v�h���b�v���Ă���ύX�����f�����)
        ImGui::SliderFloat("bounceScaleX",   &bounceScaleX,        0.0f,  1.0f,  "%.2f");   // ���˂�Ƃ���X���x�Ɋ|����l(��0.75�Ȃ猻�݂�speedX��75%�̒l�ɂ��Ă���)
        ImGui::SliderFloat("bounceScaleY",   &bounceScaleY,        0.0f,  1.0f,  "%.2f");   // ���˂�Ƃ���Y���x�Ɋ|����l(��0.75�Ȃ猻�݂�speedY��75%�̒l�ɂ��Ă���)
        ImGui::SliderInt("bounceLimit",      &bounceLimit,         0,     10);              // ���˂�ő��

        ImGui::TreePop();
    }

    ImGui::End();
}


// ���X�e�B�b�N���͒l����ړ�X�����x�N�g�����擾
const float Player::GetMoveVecX() const
{
    // ���X�e�B�b�N�̐������͏����擾
    const GamePad& gamePad = Input::Instance().GetGamePad();
    return gamePad.GetAxisLX();
}


// ���X�e�B�b�N���͒l����ړ�Y�����x�N�g�����擾
const float Player::GetMoveVecY() const
{
    // ���X�e�B�b�N�̐������͏����擾
    const GamePad& gamePad = Input::Instance().GetGamePad();
    return gamePad.GetAxisLY();
}


// ���͈ړ�
const bool Player::InputMove(const float& elapsedTime)
{
#if 0
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    
    // ����̓L�[�{�[�h����H�H
     if (gamePad.GetButton() & GamePad::BTN_RIGHT)pos.x += 0.001f;
     if (gamePad.GetButton() & GamePad::BTN_LEFT)pos.x -= 0.001f;

    float ax = gamePad.GetAxisLX();
    //if (ax == 1)pos.x += 0.001f;
    //if (ax == -1)pos.x -= 0.001f;

    model->GetTransform()->SetPosition(pos);
#endif

    // �i�s�����x�N�g���擾
    const float moveVecX = GetMoveVecX();

    // �ړ�����
    Move(moveVecX, moveSpeed);

    // �ړ��x�N�g�����[���x�N�g������Ȃ���΁i�X�V����Ă�����j�ۑ�����
    // ���ۑ����邱�ƂŃ{�^�������������Ȃ��Ă������I�ɐ��񂵂���悤�ɂȂ�
    if (moveVecX != 0.0f && moveVecX != saveMoveVecX)
    {
        saveMoveVecX = moveVecX;
    }

    // ���񏈗�(�}�u���[�L�A�j���[�V�����Đ����͏��������Ȃ�)
    if (model->GetCurrentAnimationIndex() != Anim_Break)
    {
        Turn(elapsedTime, saveMoveVecX, turnSpeed_);
    }

    // �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return (moveVecX != 0.0f);
}


// �W�����v����
const bool Player::InputJump()
{
    // �{�^�����͂ŃW�����v�i�W�����v�񐔐����t���j
    const GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & (GamePad::BTN_A | GamePad::BTN_B))
    {
        // �n�ʂɂ��Ȃ��ꍇ�̓W�����v���Ȃ�
        if (!isGround) return false;

        // �W�����v�񐔂��W�����v������ȏ�Ȃ�W�����v���Ȃ�
        if (jumpCount >= jumpLimit) return false;

        // Y���x�ɃW�����v���x��ݒ�
        //Jump(jumpSpeed);
        // �W�����v�^�C�}�[���Z�b�g
        jumpTimer = defaultJumpTime;

        // �W�����v�J�E���g���Z
        ++jumpCount;

        // �W�����v����
        return true;
    }

    return false;
}


void Player::CollisionPlayerVsEnemies()
{
    //if (invincibleTimer > 0.0f) return;

    // �o�E���X���̂ݏ������s��
    if (!isBounce) return;

    bool isHit = false;

    EnemyManager& enemyManager  = EnemyManager::Instance();
    const int     enemyCount    = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        if (Collision::IntersectAABBVsAABB(aabb_, enemy->aabb_))
        {
            isHit = true;

            // �_���[�W���󂯂Ȃ����return
            if (!enemy->ApplyDamage(1, 1.0f)) return;
        }
    }
}


void Player::UpdateDashCoolTimer(const float& elapsedTime)
{
    // �N�[���^�C�����Ȃ���Δ�΂�
    if (dashCoolTimer <= 0.0f) return;

    // �N�[���^�C������
    dashCoolTimer -= elapsedTime;
}


// ���n�����Ƃ��ɌĂ΂��֐�
void Player::OnLanding()
{
    // �W�����v�񐔃��Z�b�g
    jumpCount = 0;

    // �ړ����x�����s�ړ����x�Ɠ���(���s���)�Ȃ瑖�s�X�e�[�g�֑J��
    if (moveSpeed == runMoveSpeed)
    {
        TransitionRunState();
        return;
    }
    // �ҋ@�X�e�[�g�֑J��
    else
    {
        // ���n�A�j���[�V�����Đ�
        PlayAnimation(Anim_JumpEnd, false, 0.5f, 0.5f);

        TransitionIdleState();
        return;
    }
}


// �_�b�V�����ɌĂ΂��֐�
void Player::OnDash()
{
    // �_�b�V�����̑��x��ݒ�
    velocity.x = saveMoveVecX * dashAcceleration;
}


// �o�E���X���ɌĂ΂��֐�
void Player::OnBounce()
{
    // �ő�o�E���X�J�E���g�ȏ�Ȃ�o�E���X�I��������
    if (bounceCount >= bounceLimit)
    {
        velocity.y      = 0.0f;                 // Y���x���Z�b�g
        bounceSpeedX    = defaultBounceSpeedX;  // �o�E���XX���x���Z�b�g
        bounceSpeedY    = defaultBounceSpeedY;  // �o�E���XY���x���Z�b�g
        bounceCount     = 0;                    // �o�E���X�J�E���g���Z�b�g
        isBounce        = false;                // �o�E���X�I��
     
        OnLanding();                // ���n���̏������s��
        isGround        = true;     // ���n����
        isInvincible    = false;    // ���G���[�h����
    }
    // �o�E���X������
    else
    {    
        velocity.x    = (saveMoveVecX > 0.0f) ? bounceSpeedX : -bounceSpeedX; // �v���C���[�̌����Ă�������Ƀo�E���XX���x����    
        velocity.y    = bounceSpeedY;   // �o�E���XY���x����
        bounceSpeedX *= bounceScaleX;   // �o�E���XX���x������
        bounceSpeedY *= bounceScaleY;   // �o�E���XY���x������
        ++bounceCount;                  // �o�E���X�J�E���g���Z
    }
}


// �_���[�W���󂯂����ɌĂ΂��֐�
void Player::OnDamaged()
{
}


// ���S�����Ƃ��ɌĂ΂��֐�
void Player::OnDead()
{
    // ���ŏc�Ɉ������΂��Ă���
    GetTransform()->SetScale(DirectX::XMFLOAT3(1, 5, 1));
}


// �������E�����~�X�����Ƃ��ɌĂ΂��
void Player::OnFallDead()
{
    // ����ł����玀�S�������s��return
    if (health <= 0)
    {
        OnDead();
        //return;
    }

    // �Ō�ɒ��n�����n�`�̒[�����ɖ߂�
    {
        // �������ʒu�ɂ���Ēn�`�̍��E�ǂ���̒[�ɕ������邩���߂�
        if (aabb_.min.x <= lastLandingTerrainAABBMinX)
        {
            GetTransform()->SetPositionX(lastLandingTerrainAABBMinX + ((aabb_.max.x - aabb_.min.x) * 0.5f));
        }
        else if (aabb_.max.x >= lastLandingTerrainAABBMaxX)
        {
            GetTransform()->SetPositionX(lastLandingTerrainAABBMaxX - ((aabb_.max.x - aabb_.min.x) * 0.5f));
        }

        // �n�`�̓����Y�ʒu�ɐݒ�
        GetTransform()->SetPositionY(lastLandingTerrainAABBMaxY);

        UpdateAABB();           // �Y�ꂸ��AABB�X�V
    }

    health          -=  1;      // �̗͌���
    invincibleTimer  =  1.0f;   // ���G���Ԑݒ�

    // �W�����v���E�o�E���X���ɗ������Ƃ��̂��߂Ƀ��Z�b�g����
    {
        velocity.x      =   0.0f;                   // X���x���Z�b�g
        velocity.y      =   0.0f;                   // Y���x���Z�b�g
        bounceSpeedX    =   defaultBounceSpeedX;    // �o�E���XX���x���Z�b�g
        bounceSpeedY    =   defaultBounceSpeedY;    // �o�E���XY���x���Z�b�g
        bounceCount     =   0;                      // �o�E���X�J�E���g���Z�b�g
        isBounce        =   false;                  // �o�E���X�I��
        isInvincible    =   false;                  // ���G���[�h����

        gravity         =   defaultGravity;         // �d�̓��Z�b�g

        OnLanding();                                // ���n���̏������s��
        isGround        =   true;                   // ���n����
    }

    TransitionIdleState();      // �ҋ@�X�e�[�g�֑J��
}


// �ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
    state = State::Idle;

    // ���n�A�j���[�V�������Đ�����Ă��Ȃ���Αҋ@�A�j���[�V�����Đ�
    if (model->GetCurrentAnimationIndex() != Anim_JumpEnd) PlayAnimation(Anim_Idle, true);
}

// �ҋ@�X�e�[�g�X�V����
void Player::UpdateIdleState(const float& elapsedTime)
{
    // ���n�A�j���[�V�������I��������ҋ@�A�j���[�V�����Đ��i���[�v���Ȃ����n�A�j���[�V�����̂Ƃ��̂ݏ������s����j
    if (!IsPlayAnimation()) PlayAnimation(Anim_Idle, true);

    // �W�����v���͏���(�W�����v���Ă�����W�����v�X�e�[�g�֑J��)
    if (InputJump())
    {
        TransitionJumpState();
        return;
    }
    // �ړ����͏���
    else if (InputMove(elapsedTime))
    {
        // �_�b�V���L�[�������ꂽ�u�ԂȂ�_�b�V���X�e�[�g�֑J��
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & (GamePad::BTN_X | GamePad::BTN_Y))
        {
            moveSpeed       = defaultMoveSpeed; // �ړ����x�����Z�b�g
            TransitionDashState();
            return;
        }
        // �ړ����x�����s���x�Ɠ����i���s��ԁj�Ń_�b�V���L�[�������ꑱ���Ă���Α��s�X�e�[�g�֑J��
        else if (moveSpeed == runMoveSpeed && gamePad.GetButton() & (GamePad::BTN_X | GamePad::BTN_Y))
        {
            TransitionRunState();
            return;
        }
        // �ړ����͂����Ȃ���s�X�e�[�g�֑J��
        else
        {
            moveSpeed       = defaultMoveSpeed; // �ړ����x�����Z�b�g
            TransitionWalkState();
            return;
        }
    }

}


// ���s�X�e�[�g�֑J��
void Player::TransitionWalkState()
{
    state = State::Walk;

    PlayAnimation(Anim_Walk, true);
}

// ���s�X�e�[�g�X�V����
void Player::UpdateWalkState(const float& elapsedTime)
{
    // �W�����v���͏���(�W�����v���Ă�����W�����v�X�e�[�g�֑J��)
    if (InputJump())
    {
        TransitionJumpState();
        return;
    }
    else if (InputMove(elapsedTime))
    {
        // �ړ��ƃ_�b�V���L�[�����͂���Ă�����_�b�V���X�e�[�g��
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & (GamePad::BTN_X | GamePad::BTN_Y))
        {
            TransitionDashState();
            return;
        }
    }
    // �ړ����͂��Ȃ���Αҋ@�X�e�[�g�֑J��
    else
    {
        TransitionIdleState();
        return;
    }
}


// �_�b�V���X�e�[�g�֑J��
void Player::TransitionDashState()
{
    // �_�b�V���N�[���^�C�����I����Ă��Ȃ���ΑJ�ڂ��Ȃ�
    if (dashCoolTimer > 0.0f) return;

    state = State::Dash;

    isDash = true;  // �_�b�V��������

    dashCoolTimer = dashCoolTime;    // �_�b�V���N�[���^�C����ݒ�

    PlayAnimation(Anim_Dash, false, 1.0f, 0.0f);
}

// �_�b�V���X�e�[�g�X�V����
void Player::UpdateDashState(const float& elapsedTime)
{

    // ���񏈗��i�J�����ڐ��̂܂܃_�b�V�����Ȃ��悤�ɍX�V����j
    Turn(elapsedTime, saveMoveVecX, turnSpeed_);

    
    // �_�b�V���^�C�}�[���c���Ă�������_�b�V�����p��������
    if (dashTimer > 0.0f)
    {
        dashTimer -= elapsedTime;   // �^�C�}�[���Z
        return;
    }
    // �_�b�V���^�C�}�[���I�����Ă�����_�b�V���I��
    else 
    {
        dashTimer = defaultDashTime;    // �_�b�V�����Z�b�g
        isDash    = false;              // �_�b�V���I��
    }

    if (InputMove(elapsedTime))
    {
        // �ړ��ƃ_�b�V���L�[�����͂��ꑱ���Ă����瑖�s�X�e�[�g�֑J��
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButton() & (GamePad::BTN_X | GamePad::BTN_Y))
        {
            velocity.x = velocity.x * dashFinishScale; // ���x�����炷
            TransitionRunState();
            return;
        }
        // �ړ��������͂���Ă�������s�X�e�[�g�֑J��
        else
        {
            velocity.x = velocity.x * dashFinishScale; // ���x�����炷
            TransitionWalkState();
            return;
        }
    }
    // �ҋ@�X�e�[�g�֑J��
    else
    {
        velocity.x = velocity.x * dashFinishScale; // ���x�����炷
        TransitionIdleState();
        return;
    }
}


// ���s�X�e�[�g�֑J��
void Player::TransitionRunState()
{
    state = State::Run;

    // ���s���̑��x�p�����[�^��ݒ�
    moveSpeed       = runMoveSpeed;
    acceleration    = runAcceleration;
    friction        = runFriction;

    // ���s�p�ۑ��ړ��x�N�g���Ɉړ��x�N�g����ۑ�
    if (runMoveVecX == 0.0f) runMoveVecX = moveVecX_;

    // ���s�A�j���[�V�����Đ�
    PlayAnimation(Anim_Run, true, 1.0f, 0.5f);
}

// ���s�X�e�[�g�X�V����
void Player::UpdateRunState(const float& elapsedTime)
{
    // �u���[�L�A�j���[�V�������I������瑖�s�A�j���[�V�����ɖ߂�
    if(!IsPlayAnimation()) PlayAnimation(Anim_Run, true, 1.0f, 0.5f);

    // �W�����v���͏���(�W�����v���Ă�����W�����v�X�e�[�g�֑J��)
    if (InputJump())
    {   
        acceleration = defaultAcceleration;
        friction     = defaultFriction;

        runMoveVecX = moveVecX_; // �ړ��x�N�g���ۑ����X�V���Ă����i�_�b�V���W�����v���Ē��n���鐡�O�ɕ����]������ƃJ�����ڐ��Ń��W�I�̑����邽�߁j

        TransitionJumpState();
        return;
    }
    else if (InputMove(elapsedTime))
    {
        // ���x���ō����x�ɒB���Ă��āA
        // ���s�p�ۑ��ړ��x�N�g���ƌ��݂̈ړ��x�N�g���̕������Ⴄ�ꍇ�i�����]���j�̓u���[�L�A�j���[�V�������Đ�
        const bool isPlayBreakAnimation = {
            (velocity.x >=  runMoveSpeed * 0.8f && runMoveVecX > 0.0f && moveVecX_ < 0.0f) ||
            (velocity.x <= -runMoveSpeed * 0.8f && runMoveVecX < 0.0f && moveVecX_ > 0.0f)
        };
        if (isPlayBreakAnimation)
        {
            PlayAnimation(Anim_Break, false, 1.75f, 0.5f);
        }

        // ���s�L�[���͂��Ȃ���Ε��s�X�e�[�g��
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (!(gamePad.GetButton() & (GamePad::BTN_X | GamePad::BTN_Y)))
        {
            // ���x�p�����[�^�����Z�b�g
            moveSpeed    = defaultMoveSpeed;
            acceleration = defaultAcceleration;
            friction     = defaultFriction;
            TransitionWalkState();
            return;
        }

        runMoveVecX = moveVecX_; // �ۑ��ړ��x�N�g���X�V
    }
    // �ړ����͂��Ȃ���Αҋ@�X�e�[�g�֑J��
    else
    {
        //moveSpeed = defaultMoveSpeed; // �ړ����x�����Z�b�g(�ҋ@�X�e�[�g�Ń��Z�b�g���邩���߂�)
        acceleration = defaultAcceleration;
        friction     = defaultFriction;
        TransitionIdleState();
        return;
    }
}


// �W�����v�X�e�[�g�֑J��
void Player::TransitionJumpState()
{
    state = State::Jump;

    // �W�����v�����ʒu��ۑ�
    jumpedPositionY = GetTransform()->GetPosition().y;
    
    // �W�����v�J�n�A�j���[�V�����Đ�
    PlayAnimation(Anim_JumpInit, false, 1.0f, 0.5f);
}

// �W�����v�X�e�[�g�X�V����
void Player::UpdateJumpState(const float& elapsedTime)
{
    // �W�����v�J�n�A�j���[�V�������I��������W�����v�A�j���[�V�����Đ�
    if (!IsPlayAnimation()) PlayAnimation(Anim_Jump, true, 1.25f);

    // �������n�߂��痎���A�j���[�V�����Đ�
    if (velocity.y < 0.0f) PlayAnimation(Anim_Fall, true);

    // �ړ����͏���
    InputMove(elapsedTime);

    // �W�����v���͏���
    InputJump();

    // �W�����v�^�C�}�[������ꍇ�i�W�����v���j
    if (jumpTimer > 0.0f)
    {
        // �W�����v�{�^��������������ƍ������
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButton() & (GamePad::BTN_A | GamePad::BTN_B))
        {
            Jump(jumpSpeed);            // �W�����v����
            jumpTimer -= elapsedTime;   // �W�����v�^�C�}�[���Z
        }
        // �W�����v�{�^���𗣂�����W�����v�I��
        else
        {
            jumpTimer = 0.0f;           // �W�����^�C�}�[���Z�b�g
        }
    }

    // �������ɉ�����Ă�����
    if (GetMoveVecY() < 0.0f)
    {      
        // �W�����v�J�n����Y�ʒu�ƌ��݂�Y�ʒu����W�����v�����������Z�o
        const float& vecY = GetTransform()->GetPosition().y - jumpedPositionY;
        const float jumpHeight = sqrtf(vecY * vecY);

        // �q�b�v�h���b�v�ɕK�v�ȋ���(����)�ɒB���Ă�����q�b�v�h���b�v�X�e�[�g�֑J��
        if (jumpHeight >= needHipDropHeight)
        {
            TransitionHipDropState();
            return;
        }
    }
}


// �q�b�v�h���b�v�X�e�[�g�֑J��
void Player::TransitionHipDropState()
{
    state = State::HipDrop;

    //// �v���C���[�̐��K���ړ�X�x�N�g�������߂�(�X�e�[�g�J�ڎ��Ɍv�Z����)
    //const float length = sqrtf(saveMoveVecX * saveMoveVecX);
    //saveMoveVec_n      = saveMoveVecX / length;

    gravity         = hipDropGravity;  // �������x���㏸
    isBounce        = true;            // �o�E���X������
    isInvincible    = true;            // �q�b�v�h���b�v�o�E���X���͖��G���[�h�ɂ���

    PlayAnimation(Anim_HipDrop, true);
}

// �q�b�v�h���b�v�X�e�[�g�X�V����
void Player::UpdateHipDropState(const float& elapsedTime)
{
    // ���񏈗��i�J�����ڐ��̂܂܃o�E���X���Ȃ��悤�ɍX�V����j
    Turn(elapsedTime, saveMoveVecX, turnSpeed_);

    // ���o�E���X������d�͂����Ƃɖ߂�
    if (bounceCount != 0) gravity = defaultGravity;

    //// �o�E���X���I��������ҋ@�X�e�[�g�֑J��
    //if (!isBounce)
    //{
    //    TransitionIdleState();
    //    return;
    //}

}
