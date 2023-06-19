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

    // 待機ステートへ遷移
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
    // GamePad取得
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


    // ステート分岐処理
    switch (state)
    {
    case State::Idle:    UpdateIdleState(elapsedTime);    break;
    case State::Move:    UpdateMoveState(elapsedTime);    break;
    case State::Jump:    UpdateJumpState(elapsedTime);    break;
    case State::HipDrop: UpdateHipDropState(elapsedTime); break;
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

    ImGui::SliderFloat("turnSpeed",      &turnSpeed,      0.0f, ToRadian(900.0f), "%.0f");  // 旋回速度
    ImGui::SliderFloat("hipDropGravity", &hipDropGravity, 0.0f, -10.0f,           "%.0f");  // ヒップドロップ時の重力

    // バウンスパラメータ
    if (ImGui::TreeNode("Bounce"))
    {
        ImGui::SliderFloat("d_BounceSpeedX", &defaultBounceSpeedX, 0.0f,  30.0f, "%.0f");   // 跳ねるときのX速度(※一回ヒップドロップしてから変更が反映される)
        ImGui::SliderFloat("d_BounceSpeedY", &defaultBounceSpeedY, 0.0f,  30.0f, "%.0f");   // 跳ねるときのY速度(※一回ヒップドロップしてから変更が反映される)
        ImGui::SliderFloat("bounceScaleX",   &bounceScaleX,        0.0f,  1.0f,  "%.2f");   // 跳ねるときのX速度に掛ける値(※0.75なら現在のspeedXを75%の値にしていく)
        ImGui::SliderFloat("bounceScaleY",   &bounceScaleY,        0.0f,  1.0f,  "%.2f");   // 跳ねるときのY速度に掛ける値(※0.75なら現在のspeedYを75%の値にしていく)
        ImGui::SliderInt("bounceLimit",      &bounceLimit,         0,     10);              // 跳ねる最大回数
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


// 左スティック入力値から移動X方向ベクトルを取得
const float Player::GetMoveVecX() const
{
    // 左スティックの水平入力情報を取得
    const GamePad& gamePad = Input::Instance().GetGamePad();
    return gamePad.GetAxisLX();
}


// 左スティック入力値から移動Y方向ベクトルを取得
const float Player::GetMoveVecY() const
{
    // 左スティックの垂直入力情報を取得
    const GamePad& gamePad = Input::Instance().GetGamePad();
    return gamePad.GetAxisLY();
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
    const float moveVecX = GetMoveVecX();

    // 移動処理
    Move(moveVecX, moveSpeed);

    // 移動ベクトルがゼロベクトルじゃなければ（更新されていたら）保存する
    // ※保存することでボタンを押し続けなくても自動的に旋回する
    if (moveVecX != 0.0f) saveMoveVecX = moveVecX; 

    // 旋回処理
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // 進行ベクトルがゼロベクトルでない場合は入力された
    return (moveVecX != 0.0f);
}


// ジャンプ入力
const bool Player::InputJump()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    // ボタン入力でジャンプ（ジャンプ回数制限付き）
    //if (gamePad.GetButtonDown() & GamePad::BTN_A)
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
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


// 着地したときに呼ばれる関数
void Player::OnLanding()
{
    // ジャンプ回数リセット
    jumpCount = 0;

    // 待機ステートへ遷移
    TransitionIdleState();
}


// バウンス時に呼ばれる関数
void Player::OnBounce()
{
    // 最大バウンスカウント以上ならバウンス終了させる
    if (bounceCount >= bounceLimit)
    {
        velocity.y      = 0;                    // Y速度リセット
        bounceSpeedX    = defaultBounceSpeedX;  // バウンスX速度リセット
        bounceSpeedY    = defaultBounceSpeedY;  // バウンスY速度リセット
        bounceCount     = 0;                    // バウンスカウントリセット
        isBounce        = false;                // バウンス終了
    }
    // バウンスさせる
    else
    {
        velocity.x    = saveMoveVec_n * bounceSpeedX;   // プレイヤーの向いている方向にバウンスX速度を代入
        //velocity.y    = -velocity.y * bounceScaleY;   // 反転して減少させたY速度を代入
        velocity.y    = bounceSpeedY;
        bounceSpeedX *= bounceScaleX;                   // バウンスX速度を減少
        bounceSpeedY *= bounceScaleY;                   // バウンスY速度を減少
        ++bounceCount;                                  // バウンスカウント加算
    }
}


// ダメージを受けた時に呼ばれる関数
void Player::OnDamaged()
{
}


// 死亡したときに呼ばれる関数
void Player::OnDead()
{
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

    // 下方向に押されていたらヒップドロップステートへ遷移
    if (GetMoveVecY() < 0.0f) TransitionHipDropState();
}


// ヒップドロップステートへ遷移
void Player::TransitionHipDropState()
{
    state = State::HipDrop;

    // プレイヤーの正規化移動ベクトルを求める(ステート遷移時に一回だけ計算する)
    const float length    = sqrtf(saveMoveVecX * saveMoveVecX);
    saveMoveVec_n         = saveMoveVecX / length;

    gravity  = hipDropGravity;  // 落下速度を上昇
    isBounce = true;            // バウンスさせる
}

// ヒップドロップステート更新処理
void Player::UpdateHipDropState(const float& elapsedTime)
{
    // 旋回処理（カメラ目線のままバウンドしないように更新する）
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // 一回バウンスしたら重力をもとに戻す
    if (bounceCount != 0) gravity = defaultGravity;

    // バウンスが終了したら待機ステートへ遷移
    if (!isBounce)
    {
        gravity = defaultGravity; // 重力を元に戻す
        TransitionIdleState();
    }

}
