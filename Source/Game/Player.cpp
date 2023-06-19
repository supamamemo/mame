#include "Player.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"
#include "OperatorXMFLOAT3.h"

Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/enemy_001Ver10.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/castel.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/jump.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/idletest.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/hiyokomame.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temp.fbx", true);
    

    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/byoga/plantune.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/byoga/nico.fbx", true);

    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/nopark.fbx", true);


    //geometricPrimitive = std::make_unique<GeometricPrimitive>(graphics.GetDevice());
    
    create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/wireframe.cso", pixel_shaders.GetAddressOf());

    //sprite = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/load.png");

    // �ҋ@�X�e�[�g�֑J��
    TransitionIdleState();

    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    box2d = { {pos.x,pos.y},{0.5f,0.5f} };
    //aabb = { {pos.x,pos.y,pos.z},{0.5f,0.5f,0.5f} };
}


Player::~Player()
{
}


void Player::Initialize()
{
    GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 10));
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(180), 0, 0));
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

    //debugModel->GetTransform()->SetPosition(model->GetTransform()->GetPosition());
    //debugModel->GetTransform()->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));


    // �X�e�[�g���򏈗�
    switch (state)
    {
    case State::Idle:    UpdateIdleState(elapsedTime);    break;
    case State::Move:    UpdateMoveState(elapsedTime);    break;
    case State::Jump:    UpdateJumpState(elapsedTime);    break;
    case State::HipDrop: UpdateHipDropState(elapsedTime); break;
    }

    // ���͍X�V����
    UpdateVelocity(elapsedTime);

    // ���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);
}


void Player::End()
{
}


void Player::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    // Transform�X�V
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��
    if (model->skinned_meshes.animation_clips.size() > 0)
    {
        int clip_index{ GetAnimation() };
        int frame_index{ 0 };
        static float animation_tick{ 0 };

        animation& animation{ model->skinned_meshes.animation_clips.at(clip_index) };
        frame_index = static_cast<int>(animation_tick * animation.sampling_rate);
        if (frame_index > animation.sequence.size() - 1)
        {
            frame_index = 0;
            animation_tick = 0;
        }
        else
        {
            animation_tick += elapsedTime;
        }
        animation::keyframe& keyframe{ animation.sequence.at(frame_index) };

        model->skinned_meshes.render(immediate_context, transform, DirectX::XMFLOAT4(1, 1, 1, 1), &keyframe);
    }
    else
    {
        model->skinned_meshes.render(immediate_context, transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
    }

    // BOUNDING_BOX
    {
        DirectX::XMFLOAT4X4 t = SetDebugModelTransform(transform);
        debugModel->skinned_meshes.render(graphics.GetDeviceContext(), t, { 1.0f, 0.0f, 0.0f, 0.2f }, nullptr);
    }

    Shader* shader = graphics.GetShader();
    shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);
}



void Player::DrawDebug()
{
    ImGui::Begin("player");
    
    Character::DrawDebug();

    //ImGui::SliderInt("animationIndex", &animationIndex, 0, 2);

    ImGui::DragFloat2("box2dLenght", &box2d.lenght.x);


    ImGui::SliderInt("animationIndex", &animationIndex, 0, 2);

    ImGui::SliderFloat("turnSpeed",      &turnSpeed,      0.0f, ToRadian(900.0f), "%.0f");  // ���񑬓x
    ImGui::SliderFloat("hipDropGravity", &hipDropGravity, 0.0f, -10.0f,           "%.0f");  // �q�b�v�h���b�v���̏d��

    // �o�E���X�p�����[�^
    if (ImGui::TreeNode("Bounce"))
    {
        ImGui::SliderFloat("d_BounceSpeedX", &defaultBounceSpeedX, 0.0f,  30.0f, "%.0f");   // ���˂�Ƃ���X���x(�����q�b�v�h���b�v���Ă���ύX�����f�����)
        ImGui::SliderFloat("d_BounceSpeedY", &defaultBounceSpeedY, 0.0f,  30.0f, "%.0f");   // ���˂�Ƃ���Y���x(�����q�b�v�h���b�v���Ă���ύX�����f�����)
        ImGui::SliderFloat("bounceScaleX",   &bounceScaleX,        0.0f,  1.0f,  "%.2f");   // ���˂�Ƃ���X���x�Ɋ|����l(��0.75�Ȃ猻�݂�speedX��75%�̒l�ɂ��Ă���)
        ImGui::SliderFloat("bounceScaleY",   &bounceScaleY,        0.0f,  1.0f,  "%.2f");   // ���˂�Ƃ���Y���x�Ɋ|����l(��0.75�Ȃ猻�݂�speedY��75%�̒l�ɂ��Ă���)
        ImGui::SliderInt("bounceLimit",      &bounceLimit,         0,     10);              // ���˂�ő��
        ImGui::TreePop();

        if (ImGui::Button("Bounce Reset"))
        {
            defaultBounceSpeedX = 15.0f;
            defaultBounceSpeedY = 10.0f;
            bounceScaleX        = 0.75f;
            bounceScaleY        = 0.75f;
            bounceLimit         = 3;
        }
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
    if (moveVecX != 0.0f) saveMoveVecX = moveVecX; 

    // ���񏈗�
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return (moveVecX != 0.0f);
}


// �W�����v����
const bool Player::InputJump()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    // �{�^�����͂ŃW�����v�i�W�����v�񐔐����t���j
    //if (gamePad.GetButtonDown() & GamePad::BTN_A)
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        // �W�����v�񐔂��W�����v������ȏ�Ȃ�W�����v���Ȃ�
        if (jumpCount >= jumpLimit) return false;

        // Y���x�ɃW�����v���x��ݒ�
        Jump(jumpSpeed);

        // �W�����v�J�E���g���Z
        ++jumpCount;

        // �W�����v����
        return true;
    }

    return false;
}


// ���n�����Ƃ��ɌĂ΂��֐�
void Player::OnLanding()
{
    // �W�����v�񐔃��Z�b�g
    jumpCount = 0;

    // �ҋ@�X�e�[�g�֑J��
    TransitionIdleState();
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
        //velocity.y    = -velocity.y * bounceScaleY;   // ���]���Č���������Y���x����
        velocity.y    = bounceSpeedY;
        bounceSpeedX *= bounceScaleX;                   // �o�E���XX���x������
        bounceSpeedY *= bounceScaleY;                   // �o�E���XY���x������
        ++bounceCount;                                  // �o�E���X�J�E���g���Z
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

    // �ҋ@�A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Idle, true);
}

// �ҋ@�X�e�[�g�X�V����
void Player::UpdateIdleState(const float& elapsedTime)
{
    // �ړ����͏���(���͂�����Έړ��X�e�[�g�֑J��)
    if (InputMove(elapsedTime)) TransitionMoveState();

    // �W�����v���͏���(�W�����v���Ă�����W�����v�X�e�[�g�֑J��)
    if (InputJump()) TransitionJumpState();
}


// �ړ��X�e�[�g�֑J��
void Player::TransitionMoveState()
{
    state = State::Move;

    //model->PlayAnimation(Anim_Running, true);
}

// �ړ��X�e�[�g�X�V����
void Player::UpdateMoveState(const float& elapsedTime)
{
    // �ړ����͏���(���͂��Ȃ���Αҋ@�X�e�[�g�֑J��)
    if (InputMove(elapsedTime) == false) TransitionIdleState();

    // �W�����v���͏���(�W�����v���Ă�����W�����v�X�e�[�g�֑J��)
    if (InputJump()) TransitionJumpState();
}


// �W�����v�X�e�[�g�֑J��
void Player::TransitionJumpState()
{
    state = State::Jump;
    

    //model->PlayAnimation(Anim_Jump, false);
}

// �W�����v�X�e�[�g�X�V����
void Player::UpdateJumpState(const float& elapsedTime)
{
    // �ړ����͏���
    InputMove(elapsedTime);

    // �������ɉ�����Ă�����q�b�v�h���b�v�X�e�[�g�֑J��
    if (GetMoveVecY() < 0.0f) TransitionHipDropState();
}


// �q�b�v�h���b�v�X�e�[�g�֑J��
void Player::TransitionHipDropState()
{
    state = State::HipDrop;

    // �v���C���[�̐��K���ړ��x�N�g�������߂�(�X�e�[�g�J�ڎ��Ɉ�񂾂��v�Z����)
    const float length    = sqrtf(saveMoveVecX * saveMoveVecX);
    saveMoveVec_n         = saveMoveVecX / length;

    gravity  = hipDropGravity;  // �������x���㏸
    isBounce = true;            // �o�E���X������
}

// �q�b�v�h���b�v�X�e�[�g�X�V����
void Player::UpdateHipDropState(const float& elapsedTime)
{
    // ���񏈗��i�J�����ڐ��̂܂܃o�E���h���Ȃ��悤�ɍX�V����j
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // ���o�E���X������d�͂����Ƃɖ߂�
    if (bounceCount != 0) gravity = defaultGravity;

    // �o�E���X���I��������ҋ@�X�e�[�g�֑J��
    if (!isBounce)
    {
        gravity = defaultGravity; // �d�͂����ɖ߂�
        TransitionIdleState();
    }

}
