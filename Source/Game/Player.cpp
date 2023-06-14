#include "Player.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"

Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    //model = new Model(graphics.GetDevice(), "./resources/idle3.fbx", false);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/idletest.fbx", true);
    //model = new Model(graphics.GetDevice(), "./resources/idletest.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/nopark.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/hiyokomame.fbx", true);
    //model = new Model(graphics.GetDevice(), "./resources/hiyokomame.fbx", true);    //model = new Model(graphics.GetDevice(), "./resources/mame.fbx", 0, true);
    //model = new Model(graphics.GetDevice(), "./resources/byoga/plantune.fbx", 0, true);
    //model = new Model(graphics.GetDevice(), "/resources/byoga/nico.fbx");

    // �ҋ@�X�e�[�g�֑J��
    TransitionIdleState();
}

Player::~Player()
{
}

void Player::Initialize()
{
    model->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 10));
    model->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(180), 0, 0));
    model->GetTransform()->SetScale(DirectX::XMFLOAT3(3, 3, 3));
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

    // �X�e�[�g���򏈗�
    switch (state)
    {
    case State::Idle:   UpdateIdleState(elapsedTime);   break;
    case State::Move:   UpdateMoveState(elapsedTime);   break;
    case State::Jump:   UpdateJumpState(elapsedTime);   break;
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
        int clip_index{ 0 };
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
}


void Player::DrawDebug()
{
    Character::DrawDebug();
}


// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
const XMFLOAT3 Player::GetMoveVec() const
{
    // ���͏����擾
    const GamePad& gamePad = Input::Instance().GetGamePad();
    const float&   ax      = gamePad.GetAxisLX();
    const float&   ay      = gamePad.GetAxisLY();

    // �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    const Transform& cameraTransform = Camera::Instance().GetTransform();
    const XMFLOAT3&  cameraRight = cameraTransform.CalcRight();
    const XMFLOAT3&  cameraFront = cameraTransform.CalcForward();


    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���
    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    NO_CONST float cameraRightX       = cameraRight.x;
    NO_CONST float cameraRightZ       = cameraRight.z;
    const    float cameraRighthLength = sqrtf(
        cameraRightX * cameraRightX + 
        cameraRightZ * cameraRightZ
    );

    if (cameraRighthLength > 0.0f)
    {
        // �P�ʃx�N�g����
        cameraRightX /= cameraRighthLength;
        cameraRightZ /= cameraRighthLength;
    }

    // �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    NO_CONST float cameraFrontX      = cameraFront.x;
    NO_CONST float cameraFrontZ      = cameraFront.z;
    const    float cameraFrontLength = sqrtf(
        cameraFrontX * cameraFrontX + 
        cameraFrontZ * cameraFrontZ
    );

    if (cameraFrontLength > 0.0f)
    {
        // �P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    // �i�s�x�N�g�����v�Z����
    const XMFLOAT3 vec = {
       (cameraRightX * ax) + (cameraFrontX * ay),
       0.0f, // Y�������ɂ͈ړ����Ȃ�
       (cameraRightZ * ax) + (cameraFrontZ * ay),       
    };

    return vec;
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
    const XMFLOAT3& moveVec = GetMoveVec();

    // �ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    // ���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    // �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return (moveVec.x != 0.0f || moveVec.z != 0.0f);
}


// �W�����v����
const bool Player::InputJump()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    // �{�^�����͂ŃW�����v�i�W�����v�񐔐����t���j
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
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
}