#include "Player.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"
#include "OperatorXMFLOAT3.h"

Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // ���f������
    {
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/enemy_001Ver10.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/castel.fbx", true);
        model = std::make_unique<Model>(graphics.GetDevice(), "./resources/matome0620_1.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/jump.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/idletest.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/hiyokomame.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temp.fbx", true);
    }

    //geometricPrimitive = std::make_unique<GeometricPrimitive>(graphics.GetDevice());
    
    create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/wireframe.cso", pixel_shaders.GetAddressOf());

    //sprite = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/load.png");

    // �ҋ@�X�e�[�g�֑J��
    TransitionIdleState();

    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
}


Player::~Player()
{
}


void Player::Initialize()
{
    GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 10));
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(180), 0, 0));
    GetTransform()->SetScale(DirectX::XMFLOAT3(1, 1, 1));

    debugModel->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 10));
}


void Player::Finalize()
{
}


void Player::Begin()
{
    
}


void Player::Update(const float& elapsedTime)
{  
#if 0
    // GamePad�擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    if (gamePad.GetButton() & GamePad::BTN_B)
    {
        pos.x += 0.01f;
    }
    model->GetTransform()->SetPosition(pos);
#endif

    debugModel->GetTransform()->SetPosition(model->GetTransform()->GetPosition());
    debugModel->GetTransform()->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));

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

    // ���͍X�V����
    UpdateVelocity(elapsedTime);

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
    Graphics& graphics = Graphics::Instance();

    // Transform�X�V
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��
    if (&keyframe)
    {
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
        debugModel->skinned_meshes.render(graphics.GetDeviceContext(), t, { 1.0f, 0.0f, 0.0f, 0.2f }, nullptr);
    }
#endif // _DEBUG
}


// �f�o�b�O�`��
void Player::DrawDebug()
{
    ImGui::Begin("player");

    ImGui::Checkbox("isDebugBlendAnimation", &isDebugBlendAnimation);

    ImGui::SliderInt("animationIndex", &currentAnimationIndex, 0, Anim_Max - 1);

    ImGui::DragFloat2("box2dLenght", &box2d.lenght.x);

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

        ImGui::SliderFloat("turnSpeed",        &turnSpeed, 1.0f, ToRadian(900.0f), "%.0f");  // ���񑬓x

        if (ImGui::Button("Speed Reset"))
        {
            velocity            =  {};
            acceleration        =  1.0f;
            defaultGravity      = -1.0f;
            hipDropGravity      = -3.0f;
            friction            =  0.5f;
            airControl          =  0.3f;

            dashAcceleration    =  30.0f;
            defaultMoveSpeed    =  5.0f;
            runMoveSpeed        =  15.0f;
                                   
            turnSpeed           =  ToRadian(900.0f);
        }

        ImGui::TreePop();
    }

    // �W�����v�֘A�p�����[�^
    if (ImGui::TreeNode("Jump"))
    {
        ImGui::SliderFloat("jumpSpeed",  &jumpSpeed,       0.0f, 30.0f);   // �W�����v���x
        ImGui::SliderFloat("d_JumpTime", &defaultJumpTime, 0.0f, 1.0f);    // �W�����v���������鎞��
        ImGui::SliderInt("jumpLimit",    &jumpLimit,       1,    5);       // �W�����v�ő��

        if (ImGui::Button("Jump Reset"))
        {
            jumpSpeed       = 10.0f;
            defaultJumpTime = 0.3f;
            jumpLimit       = 1;
        }

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

        if (ImGui::Button("Bounce Reset"))
        {
            defaultBounceSpeedX = 15.0f;
            defaultBounceSpeedY = 10.0f;
            bounceScaleX        = 0.75f;
            bounceScaleY        = 0.75f;
            bounceLimit         = 2;
        }

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
    // ���ۑ����邱�ƂŃ{�^�������������Ȃ��Ă������I�ɐ��񂷂�
    if (moveVecX != 0.0f && moveVecX != saveMoveVecX)
    {
        saveMoveVecX = moveVecX;
    }

    // ���񏈗�
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return (moveVecX != 0.0f);
}


// �W�����v����
const bool Player::InputJump()
{
    // �{�^�����͂ŃW�����v�i�W�����v�񐔐����t���j
    const GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
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

    // ���n�A�j���[�V�����Đ�
    PlayAnimation(Anim_JumpEnd, false, 0.5f);

    // �ړ����x�����s�ړ����x�Ɠ���(���s���)�Ȃ瑖�s�X�e�[�g�֑J��
    if (moveSpeed == runMoveSpeed)
    {
        TransitionRunState();
        return;
    }
    // �ҋ@�X�e�[�g�֑J��
    else
    {
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
        velocity.y      = 0;                    // Y���x���Z�b�g
        bounceSpeedX    = defaultBounceSpeedX;  // �o�E���XX���x���Z�b�g
        bounceSpeedY    = defaultBounceSpeedY;  // �o�E���XY���x���Z�b�g
        bounceCount     = 0;                    // �o�E���X�J�E���g���Z�b�g
        isBounce        = false;                // �o�E���X�I��
    }
    // �o�E���X������
    else
    {
        velocity.x    = saveMoveVec_n * bounceSpeedX;   // �v���C���[�̌����Ă�������Ƀo�E���XX���x����
        velocity.y    = bounceSpeedY;                   // �o�E���XY���x����
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
}


// �ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
    state = State::Idle;

    // ���n�A�j���[�V�������Đ�����Ă��Ȃ���Αҋ@�A�j���[�V�����Đ�
    if (currentAnimationIndex != Anim_JumpEnd) PlayAnimation(Anim_Idle, true);
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
        if (gamePad.GetButtonDown() & GamePad::BTN_X)
        {
            moveSpeed = defaultMoveSpeed; // �ړ����x�����Z�b�g
            TransitionDashState();
            return;
        }
        // �ړ����x�����s���x�Ɠ����i���s��ԁj�Ń_�b�V���L�[�������ꑱ���Ă���Α��s�X�e�[�g�֑J��
        else if (moveSpeed == runMoveSpeed && gamePad.GetButton() & GamePad::BTN_X)
        {
            TransitionRunState();
            return;
        }
        // �ړ����͂����Ȃ���s�X�e�[�g�֑J��
        else
        {
            moveSpeed = defaultMoveSpeed; // �ړ����x�����Z�b�g
            TransitionWalkState();
            return;
        }
    }

}


// ���s�X�e�[�g�֑J��
void Player::TransitionWalkState()
{
    state = State::Walk;

    PlayAnimation(Anim_Idle, true);
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
        if (gamePad.GetButtonDown() & GamePad::BTN_X)
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

    PlayAnimation(Anim_Dash, false);
}

// �_�b�V���X�e�[�g�X�V����
void Player::UpdateDashState(const float& elapsedTime)
{
    // ���񏈗��i�J�����ڐ��̂܂܃_�b�V�����Ȃ��悤�ɍX�V����j
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

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
        if (gamePad.GetButton() & GamePad::BTN_X)
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

    // ���s���̈ړ����x�ɐݒ�
    moveSpeed = runMoveSpeed;

    // ���n�A�j���[�V�������Đ�����Ă��Ȃ���Α��s�A�j���[�V�����Đ�
    if (currentAnimationIndex != Anim_JumpEnd) PlayAnimation(Anim_Run, true);
}

// ���s�X�e�[�g�X�V����
void Player::UpdateRunState(const float& elapsedTime)
{
    // ���n�A�j���[�V�������I�������瑖�s�A�j���[�V�����Đ��i���[�v���Ȃ����n�A�j���[�V�����̂Ƃ��̂ݏ������s����j
    if(!IsPlayAnimation()) PlayAnimation(Anim_Run, true);

    // �W�����v���͏���(�W�����v���Ă�����W�����v�X�e�[�g�֑J��)
    if (InputJump())
    {   
        TransitionJumpState();
        return;
    }
    else if (InputMove(elapsedTime))
    {
        // ���s�L�[���͂��Ȃ���Ε��s�X�e�[�g��
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (!(gamePad.GetButton() & GamePad::BTN_X))
        {
            moveSpeed = defaultMoveSpeed; // �ړ����x�����Z�b�g
            TransitionWalkState();
            return;
        }
    }
    // �ړ����͂��Ȃ���Αҋ@�X�e�[�g�֑J��
    else
    {
        //moveSpeed = defaultMoveSpeed; // �ړ����x�����Z�b�g(�ҋ@�X�e�[�g�Ń��Z�b�g���邩���߂�)
        TransitionIdleState();
        return;
    }
}


// �W�����v�X�e�[�g�֑J��
void Player::TransitionJumpState()
{
    state = State::Jump;
    
    // �W�����v�J�n�A�j���[�V�����Đ�
    PlayAnimation(Anim_JumpInit, false);
}

// �W�����v�X�e�[�g�X�V����
void Player::UpdateJumpState(const float& elapsedTime)
{
    // �W�����v�J�n�A�j���[�V�������I��������W�����v�A�j���[�V�����Đ�
    if (!IsPlayAnimation()) PlayAnimation(Anim_Jump, true);

    // �ړ����͏���
    InputMove(elapsedTime);

    // �W�����v���͏���
    InputJump();

    // �W�����v�^�C�}�[������ꍇ�i�W�����v���j
    if (jumpTimer > 0.0f)
    {
        // �W�����v�{�^��������������ƍ������
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButton() & GamePad::BTN_B)
        {
            Jump(jumpSpeed);            // �W�����v����
            jumpTimer -= elapsedTime;   // �W�����v�^�C�}�[���Z
        }
        // �W�����v�{�^���𗣂�����W�����v�I��
        else
        {
            jumpTimer = 0.0f;               // �W�����^�C�}�[���Z�b�g
            PlayAnimation(Anim_Fall, true); // �����A�j���[�V�����Đ�
        }
    }

    // �������ɉ�����Ă�����q�b�v�h���b�v�X�e�[�g�֑J��
    if (GetMoveVecY() < 0.0f)
    {
        TransitionHipDropState();
        return;
    }
}


// �q�b�v�h���b�v�X�e�[�g�֑J��
void Player::TransitionHipDropState()
{
    state = State::HipDrop;

    // �v���C���[�̐��K���ړ�X�x�N�g�������߂�(�X�e�[�g�J�ڎ��Ɍv�Z����)
    const float length = sqrtf(saveMoveVecX * saveMoveVecX);
    saveMoveVec_n      = saveMoveVecX / length;

    gravity  = hipDropGravity;  // �������x���㏸
    isBounce = true;            // �o�E���X������

    PlayAnimation(Anim_HipDrop, true);
}

// �q�b�v�h���b�v�X�e�[�g�X�V����
void Player::UpdateHipDropState(const float& elapsedTime)
{
    // ���񏈗��i�J�����ڐ��̂܂܃o�E���X���Ȃ��悤�ɍX�V����j
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // ���o�E���X������d�͂����Ƃɖ߂�
    if (bounceCount != 0) gravity = defaultGravity;

    // �o�E���X���I��������ҋ@�X�e�[�g�֑J��
    if (!isBounce)
    {
        TransitionIdleState();
        return;
    }

}
