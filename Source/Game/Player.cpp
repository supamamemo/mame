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

    // 待機ステートへ遷移
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
    // GamePad取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    if (gamePad.GetButton() & GamePad::BTN_B)
    {
        pos.x += 0.01f;
    }
    model->GetTransform()->SetPosition(pos);
#endif

    // ステート分岐処理
    switch (state)
    {
    case State::Idle:   UpdateIdleState(elapsedTime);   break;
    case State::Move:   UpdateMoveState(elapsedTime);   break;
    case State::Jump:   UpdateJumpState(elapsedTime);   break;
    }

    // 速力更新処理
    UpdateVelocity(elapsedTime);

    // 無敵時間更新
    UpdateInvincibleTimer(elapsedTime);
}

void Player::End()
{
}


void Player::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    // Transform更新
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
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


// スティック入力値から移動ベクトルを取得
const XMFLOAT3 Player::GetMoveVec() const
{
    // 入力情報を取得
    const GamePad& gamePad = Input::Instance().GetGamePad();
    const float&   ax      = gamePad.GetAxisLX();
    const float&   ay      = gamePad.GetAxisLY();

    // カメラ方向とスティックの入力値によって進行方向を計算する
    const Transform& cameraTransform = Camera::Instance().GetTransform();
    const XMFLOAT3&  cameraRight = cameraTransform.CalcRight();
    const XMFLOAT3&  cameraFront = cameraTransform.CalcForward();


    // 移動ベクトルはXZ平面に水平なベクトルになるようにする
    // カメラ右方向ベクトルをXZ単位ベクトルに変換
    NO_CONST float cameraRightX       = cameraRight.x;
    NO_CONST float cameraRightZ       = cameraRight.z;
    const    float cameraRighthLength = sqrtf(
        cameraRightX * cameraRightX + 
        cameraRightZ * cameraRightZ
    );

    if (cameraRighthLength > 0.0f)
    {
        // 単位ベクトル化
        cameraRightX /= cameraRighthLength;
        cameraRightZ /= cameraRighthLength;
    }

    // カメラ前方向ベクトルをXZ単位ベクトルに変換
    NO_CONST float cameraFrontX      = cameraFront.x;
    NO_CONST float cameraFrontZ      = cameraFront.z;
    const    float cameraFrontLength = sqrtf(
        cameraFrontX * cameraFrontX + 
        cameraFrontZ * cameraFrontZ
    );

    if (cameraFrontLength > 0.0f)
    {
        // 単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    // スティックの水平入力値をカメラ右方向に反映し、
    // スティックの垂直入力値をカメラ前方向に反映し、
    // 進行ベクトルを計算する
    const XMFLOAT3 vec = {
       (cameraRightX * ax) + (cameraFrontX * ay),
       0.0f, // Y軸方向には移動しない
       (cameraRightZ * ax) + (cameraFrontZ * ay),       
    };

    return vec;
}


// 入力移動
const bool Player::InputMove(const float& elapsedTime)
{
#if 0
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    
    // これはキーボード限定？？
     if (gamePad.GetButton() & GamePad::BTN_RIGHT)pos.x += 0.001f;
     if (gamePad.GetButton() & GamePad::BTN_LEFT)pos.x -= 0.001f;

    float ax = gamePad.GetAxisLX();
    //if (ax == 1)pos.x += 0.001f;
    //if (ax == -1)pos.x -= 0.001f;

    model->GetTransform()->SetPosition(pos);
#endif

    // 進行方向ベクトル取得
    const XMFLOAT3& moveVec = GetMoveVec();

    // 移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    // 旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    // 進行ベクトルがゼロベクトルでない場合は入力された
    return (moveVec.x != 0.0f || moveVec.z != 0.0f);
}


// ジャンプ入力
const bool Player::InputJump()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    // ボタン入力でジャンプ（ジャンプ回数制限付き）
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // ジャンプ回数がジャンプ上限数以上ならジャンプしない
        if (jumpCount >= jumpLimit) return false;

        // Y速度にジャンプ速度を設定
        Jump(jumpSpeed);

        // ジャンプカウント加算
        ++jumpCount;

        // ジャンプした
        return true;
    }

    return false;
}



// 待機ステートへ遷移
void Player::TransitionIdleState()
{
    state = State::Idle;

    // 待機アニメーション再生
    //model->PlayAnimation(Anim_Idle, true);
}

// 待機ステート更新処理
void Player::UpdateIdleState(const float& elapsedTime)
{
    // 移動入力処理(入力があれば移動ステートへ遷移)
    if (InputMove(elapsedTime)) TransitionMoveState();

    // ジャンプ入力処理(ジャンプしていたらジャンプステートへ遷移)
    if (InputJump()) TransitionJumpState();
}


// 移動ステートへ遷移
void Player::TransitionMoveState()
{
    state = State::Move;

    //model->PlayAnimation(Anim_Running, true);
}

// 移動ステート更新処理
void Player::UpdateMoveState(const float& elapsedTime)
{
    // 移動入力処理(入力がなければ待機ステートへ遷移)
    if (InputMove(elapsedTime) == false) TransitionIdleState();

    // ジャンプ入力処理(ジャンプしていたらジャンプステートへ遷移)
    if (InputJump()) TransitionJumpState();
}


// ジャンプステートへ遷移
void Player::TransitionJumpState()
{
    state = State::Jump;

    //model->PlayAnimation(Anim_Jump, false);
}

// ジャンプステート更新処理
void Player::UpdateJumpState(const float& elapsedTime)
{
    // 移動入力処理
    InputMove(elapsedTime);
}