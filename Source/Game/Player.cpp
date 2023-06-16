#include "Player.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"
#include "OperatorXMFLOAT3.h"

Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/enemy_001Ver10.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/jump.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/idletest.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/hiyokomame.fbx", true);
    
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/byoga/plantune.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/byoga/nico.fbx", true);

    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/nopark.fbx", true);

    
    debugModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
    //debugModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_air_ground_move.fbx", true);

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
    model->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 10));
    model->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(180), 0, 0));
    model->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 1, 1));

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
    //DirectX::XMFLOAT4X4 transform2;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));
    //DirectX::XMStoreFloat4x4(&transform2, model->GetTransform()->CalcWorldMatrix(1.0f));

    /*DirectX::XMFLOAT4X4 transform1;
    DirectX::XMStoreFloat4x4(&transform1, debugModel->GetTransform()->CalcWorldMatrix(1.0f));*/



    //auto p = model->GetTransform()->GetPosition();
    //auto s = model->GetTransform()->GetScale();
    //auto r = model->GetTransform()->GetRotation();
    //auto sVec = DirectX::XMLoadFloat3(&s);
    //DirectX::XMVectorScale(sVec, 100);
    //DirectX::XMStoreFloat3(&s, sVec);
    //DirectX::XMMATRIX P = DirectX::XMMatrixTranslation(p.x, p.y, p.z);
    //DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(r.x, r.y, r.z);
    //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(s.x, s.y, s.z);

    //DirectX::XMStoreFloat4x4(&transform1, S * R * P);

    //geometricPrimitive->render(immediate_context, transform1, { 1,0,0,0.3f });

    // model�`��
    if (model->skinned_meshes.animation_clips.size() > 0)
    {
        int clip_index{ animationIndex };
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
        
    {
        const DirectX::XMFLOAT3 min[2]{ model->skinned_meshes.bounding_box[0] * 0.01f, debugModel->skinned_meshes.bounding_box[0] * 0.01f };
        const DirectX::XMFLOAT3 max[2]{ model->skinned_meshes.bounding_box[1] * 0.01f, debugModel->skinned_meshes.bounding_box[1] * 0.01f };
        const DirectX::XMFLOAT3 dimensions[2]
        {
            { max[0].x - min[0].x, max[0].y - min[0].y, max[0].z - min[0].z },
            { max[1].x - min[1].x, max[1].y - min[1].y, max[1].z - min[1].z }
        };
        DirectX::XMFLOAT3 relative_ratio{ dimensions[0].x / dimensions[1].x, dimensions[0].y / dimensions[1].y, dimensions[0].z / dimensions[1].z };

        DirectX::XMMATRIX O{
            DirectX::XMMatrixTranslation(dimensions[1].x / 2, dimensions[1].y / 2, dimensions[1].z / 2) *
            //DirectX::XMMatrixScaling(relative_ratio.x * 0.01f, relative_ratio.y * 0.01f, relative_ratio.z * 0.01f) *
            DirectX::XMMatrixScaling(relative_ratio.x * 0.01f, relative_ratio.y * 0.01f, relative_ratio.z * 0.01f) *
            DirectX::XMMatrixTranslation(min[0].x, min[0].y, min[0].z)
        };
        //DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scaling.x, scaling.y, scaling.z) };
        //DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
        //DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z) };
        //
        auto temp = DirectX::XMLoadFloat4x4(&transform);
        DirectX::XMFLOAT4X4 world;
        DirectX::XMStoreFloat4x4(&world, O * temp);
        debugModel->skinned_meshes.render(immediate_context, world, { 1, 0, 0, 0.3f }, nullptr, pixel_shaders.Get());
    }

    Shader* shader = graphics.GetShader();
    shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);

    

    //{
    //    const DirectX::XMFLOAT3 min[2]{ model->skinned_meshes.bounding_box[0], debugModel->skinned_meshes.bounding_box[0] };
    //    const DirectX::XMFLOAT3 max[2]{ model->skinned_meshes.bounding_box[1], debugModel->skinned_meshes.bounding_box[1] };
    //    const DirectX::XMFLOAT3 dimensions[2]
    //    {
    //        { max[0].x - min[0].x, max[0].y - min[0].y, max[0].z - min[0].z },
    //        { max[1].x - min[1].x, max[1].y - min[1].y, max[1].z - min[1].z }
    //    };
    //    DirectX::XMFLOAT3 relative_ratio{ dimensions[0].x / dimensions[1].x, dimensions[0].y / dimensions[1].y, dimensions[0].z / dimensions[1].z };

    //    DirectX::XMMATRIX O{
    //        DirectX::XMMatrixTranslation(dimensions[1].x / 2, dimensions[1].y / 2, dimensions[1].z / 2) *
    //        DirectX::XMMatrixScaling(relative_ratio.x, relative_ratio.y, relative_ratio.z) *
    //        DirectX::XMMatrixTranslation(min[0].x, min[0].y, min[0].z)
    //    };

    //    DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scaling.x, scaling.y, scaling.z) };
    //    DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
    //    DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z) };
    //    DirectX::XMFLOAT4X4 world;
    //    DirectX::XMStoreFloat4x4(&world, O * S * R * T);
    //    //debugModel->skinned_meshes.render(graphics.GetDeviceContext(), world, { 1, 0, 0, 1 }, nullptr, nullptr);
    //    debugModel->skinned_meshes.render(immediate_context, transform1  , { 1, 0, 0, 1 }, nullptr, pixel_shaders.Get());
    //    //static_meshes[1]->render(immediate_context.Get(), world, { 1, 0, 0, 1 }, pixel_shaders[0].Get());
    //}

    //debugModel->skinned_meshes.render(immediate_context, transform1, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);

    //{
    //    const DirectX::XMFLOAT3 min[2]{model->skinned_meshes.bounding_box[0],}
    //}

    //debugModel->skinned_meshes.render(immediate_context, transform1, DirectX::XMFLOAT4(0, 0, 0, 0.3f), nullptr);

    //geometricPrimitive->render(graphics.GetDeviceContext(), transform2, DirectX::XMFLOAT4(1, 0, 0, 0.3f));
}


void Player::DrawDebug()
{
    ImGui::Begin("player");
    
    Character::DrawDebug();

    ImGui::SliderInt("animationIndex", &animationIndex, 0, 2);

    ImGui::DragFloat2("box2dLenght", &box2d.lenght.x);


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
        velocity.y      = 0;                // Y���x���Z�b�g
        bounceSpeed.x   = BOUNCE_SPEED_X;   // �o�E���XX���x���Z�b�g
        bounceSpeed.y   = BOUNCE_SPEED_Y;   // �o�E���XY���x���Z�b�g
        bounceCount     = 0;                // �o�E���X�J�E���g���Z�b�g
        isBounce        = false;            // �o�E���X�I��
    }
    // �o�E���X������
    else
    {
        // �v���C���[�̑O�����̒P�ʃx�N�g�������߂�
        const float forward   = model->GetTransform()->CalcForward().x;
        const float length    = sqrtf(forward * forward);
        const float forward_n = forward / length;

        velocity.x   = forward_n * bounceSpeed.x; // �v���C���[�̌����Ă�������Ƀo�E���XX���x����
        velocity.y   = bounceSpeed.y;             // �o�E���XY���x��Y���x�ɑ��
        bounceSpeed *= BOUNCE_SPEED_DIVIDE;       // �o�E���X���x������
        ++bounceCount;                            // ���݂̃o�E���X�J�E���g���Z
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

    gravity  = BOUNCE_GRAVITY;  // �������x���㏸
    isBounce = true;            // �o�E���X������
}

// �q�b�v�h���b�v�X�e�[�g�X�V����
void Player::UpdateHipDropState(const float& elapsedTime)
{
    // ���񏈗��i�J�����ڐ��̂܂܃o�E���h���Ȃ��悤�ɍX�V����j
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // �o�E���X���I��������ҋ@�X�e�[�g�֑J��
    if (!isBounce)
    {
        gravity = GRAVITY; // �d�͂����ɖ߂�
        TransitionIdleState();
    }
}
