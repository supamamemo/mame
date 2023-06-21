#include "Player.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"
#include "OperatorXMFLOAT3.h"

Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // モデル生成
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

    // 待機ステートへ遷移
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
    // GamePad取得
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

    // ステート分岐処理
    switch (state)
    {
    case State::Idle:    UpdateIdleState(elapsedTime);    break;
    case State::Walk:    UpdateWalkState(elapsedTime);    break;
    case State::Dash:    UpdateDashState(elapsedTime);    break;
    case State::Run:     UpdateRunState(elapsedTime);     break;
    case State::Jump:    UpdateJumpState(elapsedTime);    break;
    case State::HipDrop: UpdateHipDropState(elapsedTime); break;
    }

    // 速力更新処理
    UpdateVelocity(elapsedTime);

    // 無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    // ダッシュクールタイム更新
    UpdateDashCoolTimer(elapsedTime);

    // アニメーション更新
    UpdateAnimation(elapsedTime);
}


void Player::End()
{
}


void Player::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // Transform更新
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
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


// デバッグ描画
void Player::DrawDebug()
{
    ImGui::Begin("player");

    ImGui::Checkbox("isDebugBlendAnimation", &isDebugBlendAnimation);

    ImGui::SliderInt("animationIndex", &currentAnimationIndex, 0, Anim_Max - 1);

    ImGui::DragFloat2("box2dLenght", &box2d.lenght.x);

    // 速度関連パラメータ
    if (ImGui::TreeNode("Speed"))
    {
        ImGui::InputFloat3("velocity",         &velocity.x);                      // 速度(確認用)

        ImGui::SliderFloat("acceleration",     &acceleration,     0.0f, 5.0f);    // 加速力
        ImGui::SliderFloat("d_Gravity",        &defaultGravity,   0.0f, 5.0f);    // 重力(バウンス後に反映)
        ImGui::SliderFloat("hipDropGravity",   &hipDropGravity,   1.0f, -10.0f);  // ヒップドロップ時の重力
        ImGui::SliderFloat("friction",         &friction,         0.0f, 5.0f);    // 摩擦力（停止する力）
        ImGui::SliderFloat("airControl",       &airControl,       0.0f, 1.0f);    // 空中での身動きのしやすさ

        ImGui::SliderFloat("dashAcceleration", &dashAcceleration, 0.0f, 60.0f);   // ダッシュ時の加速力
        ImGui::SliderFloat("d_MoveSpeed",      &defaultMoveSpeed, 0.0f, 20.0f);   // 通常時の最高移動速度
        ImGui::SliderFloat("runMoveSpeed",     &runMoveSpeed,     0.0f, 30.0f);   // 走行時の最高移動速度

        ImGui::SliderFloat("turnSpeed",        &turnSpeed, 1.0f, ToRadian(900.0f), "%.0f");  // 旋回速度

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

    // ジャンプ関連パラメータ
    if (ImGui::TreeNode("Jump"))
    {
        ImGui::SliderFloat("jumpSpeed",  &jumpSpeed,       0.0f, 30.0f);   // ジャンプ速度
        ImGui::SliderFloat("d_JumpTime", &defaultJumpTime, 0.0f, 1.0f);    // ジャンプし続けられる時間
        ImGui::SliderInt("jumpLimit",    &jumpLimit,       1,    5);       // ジャンプ最大回数

        if (ImGui::Button("Jump Reset"))
        {
            jumpSpeed       = 10.0f;
            defaultJumpTime = 0.3f;
            jumpLimit       = 1;
        }

        ImGui::TreePop();
    }

    // バウンス関連パラメータ
    if (ImGui::TreeNode("Bounce"))
    {
        ImGui::SliderFloat("d_BounceSpeedX", &defaultBounceSpeedX, 0.0f,  30.0f, "%.0f");   // 跳ねるときのX速度(※一回ヒップドロップしてから変更が反映される)
        ImGui::SliderFloat("d_BounceSpeedY", &defaultBounceSpeedY, 0.0f,  30.0f, "%.0f");   // 跳ねるときのY速度(※一回ヒップドロップしてから変更が反映される)
        ImGui::SliderFloat("bounceScaleX",   &bounceScaleX,        0.0f,  1.0f,  "%.2f");   // 跳ねるときのX速度に掛ける値(※0.75なら現在のspeedXを75%の値にしていく)
        ImGui::SliderFloat("bounceScaleY",   &bounceScaleY,        0.0f,  1.0f,  "%.2f");   // 跳ねるときのY速度に掛ける値(※0.75なら現在のspeedYを75%の値にしていく)
        ImGui::SliderInt("bounceLimit",      &bounceLimit,         0,     10);              // 跳ねる最大回数

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
    if (moveVecX != 0.0f && moveVecX != saveMoveVecX)
    {
        saveMoveVecX = moveVecX;
    }

    // 旋回処理
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // 進行ベクトルがゼロベクトルでない場合は入力された
    return (moveVecX != 0.0f);
}


// ジャンプ入力
const bool Player::InputJump()
{
    // ボタン入力でジャンプ（ジャンプ回数制限付き）
    const GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        // ジャンプ回数がジャンプ上限数以上ならジャンプしない
        if (jumpCount >= jumpLimit) return false;

        // Y速度にジャンプ速度を設定
        //Jump(jumpSpeed);
        // ジャンプタイマーをセット
        jumpTimer = defaultJumpTime;

        // ジャンプカウント加算
        ++jumpCount;

        // ジャンプした
        return true;
    }

    return false;
}


void Player::UpdateDashCoolTimer(const float& elapsedTime)
{
    // クールタイムがなければ飛ばす
    if (dashCoolTimer <= 0.0f) return;

    // クールタイム減少
    dashCoolTimer -= elapsedTime;
}


// 着地したときに呼ばれる関数
void Player::OnLanding()
{
    // ジャンプ回数リセット
    jumpCount = 0;

    // 着地アニメーション再生
    PlayAnimation(Anim_JumpEnd, false, 0.5f);

    // 移動速度が走行移動速度と同じ(走行状態)なら走行ステートへ遷移
    if (moveSpeed == runMoveSpeed)
    {
        TransitionRunState();
        return;
    }
    // 待機ステートへ遷移
    else
    {
        TransitionIdleState();
        return;
    }
}


// ダッシュ時に呼ばれる関数
void Player::OnDash()
{
    // ダッシュ時の速度を設定
    velocity.x = saveMoveVecX * dashAcceleration;
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
        velocity.y    = bounceSpeedY;                   // バウンスY速度を代入
        bounceSpeedX *= bounceScaleX;   // バウンスX速度を減少
        bounceSpeedY *= bounceScaleY;   // バウンスY速度を減少
        ++bounceCount;                  // バウンスカウント加算
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

    // 着地アニメーションが再生されていなければ待機アニメーション再生
    if (currentAnimationIndex != Anim_JumpEnd) PlayAnimation(Anim_Idle, true);
}

// 待機ステート更新処理
void Player::UpdateIdleState(const float& elapsedTime)
{
    // 着地アニメーションが終了したら待機アニメーション再生（ループしない着地アニメーションのときのみ処理が行われる）
    if (!IsPlayAnimation()) PlayAnimation(Anim_Idle, true);

    // ジャンプ入力処理(ジャンプしていたらジャンプステートへ遷移)
    if (InputJump())
    {
        TransitionJumpState();
        return;
    }
    // 移動入力処理
    else if (InputMove(elapsedTime))
    {
        // ダッシュキーが押された瞬間ならダッシュステートへ遷移
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & GamePad::BTN_X)
        {
            moveSpeed = defaultMoveSpeed; // 移動速度をリセット
            TransitionDashState();
            return;
        }
        // 移動速度が走行速度と同じ（走行状態）でダッシュキーが押され続けていれば走行ステートへ遷移
        else if (moveSpeed == runMoveSpeed && gamePad.GetButton() & GamePad::BTN_X)
        {
            TransitionRunState();
            return;
        }
        // 移動入力だけなら歩行ステートへ遷移
        else
        {
            moveSpeed = defaultMoveSpeed; // 移動速度をリセット
            TransitionWalkState();
            return;
        }
    }

}


// 歩行ステートへ遷移
void Player::TransitionWalkState()
{
    state = State::Walk;

    PlayAnimation(Anim_Idle, true);
}

// 歩行ステート更新処理
void Player::UpdateWalkState(const float& elapsedTime)
{
    // ジャンプ入力処理(ジャンプしていたらジャンプステートへ遷移)
    if (InputJump())
    {
        TransitionJumpState();
        return;
    }
    else if (InputMove(elapsedTime))
    {
        // 移動とダッシュキーが入力されていたらダッシュステートへ
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & GamePad::BTN_X)
        {
            TransitionDashState();
            return;
        }
    }
    // 移動入力がなければ待機ステートへ遷移
    else
    {
        TransitionIdleState();
        return;
    }
}


// ダッシュステートへ遷移
void Player::TransitionDashState()
{
    // ダッシュクールタイムが終わっていなければ遷移しない
    if (dashCoolTimer > 0.0f) return;

    state = State::Dash;

    isDash = true;  // ダッシュさせる

    dashCoolTimer = dashCoolTime;    // ダッシュクールタイムを設定

    PlayAnimation(Anim_Dash, false);
}

// ダッシュステート更新処理
void Player::UpdateDashState(const float& elapsedTime)
{
    // 旋回処理（カメラ目線のままダッシュしないように更新する）
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // ダッシュタイマーが残っていたらをダッシュを継続させる
    if (dashTimer > 0.0f)
    {
        dashTimer -= elapsedTime;   // タイマー減算
        return;
    }
    // ダッシュタイマーが終了していたらダッシュ終了
    else 
    {
        dashTimer = defaultDashTime;    // ダッシュリセット
        isDash    = false;              // ダッシュ終了
    }

    if (InputMove(elapsedTime))
    {
        // 移動とダッシュキーが入力され続けていたら走行ステートへ遷移
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButton() & GamePad::BTN_X)
        {
            velocity.x = velocity.x * dashFinishScale; // 速度を減らす
            TransitionRunState();
            return;
        }
        // 移動だけ入力されていたら歩行ステートへ遷移
        else
        {
            velocity.x = velocity.x * dashFinishScale; // 速度を減らす
            TransitionWalkState();
            return;
        }
    }
    // 待機ステートへ遷移
    else
    {
        velocity.x = velocity.x * dashFinishScale; // 速度を減らす
        TransitionIdleState();
        return;
    }
}


// 走行ステートへ遷移
void Player::TransitionRunState()
{
    state = State::Run;

    // 走行時の移動速度に設定
    moveSpeed = runMoveSpeed;

    // 着地アニメーションが再生されていなければ走行アニメーション再生
    if (currentAnimationIndex != Anim_JumpEnd) PlayAnimation(Anim_Run, true);
}

// 走行ステート更新処理
void Player::UpdateRunState(const float& elapsedTime)
{
    // 着地アニメーションが終了したら走行アニメーション再生（ループしない着地アニメーションのときのみ処理が行われる）
    if(!IsPlayAnimation()) PlayAnimation(Anim_Run, true);

    // ジャンプ入力処理(ジャンプしていたらジャンプステートへ遷移)
    if (InputJump())
    {   
        TransitionJumpState();
        return;
    }
    else if (InputMove(elapsedTime))
    {
        // 走行キー入力がなければ歩行ステートへ
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (!(gamePad.GetButton() & GamePad::BTN_X))
        {
            moveSpeed = defaultMoveSpeed; // 移動速度をリセット
            TransitionWalkState();
            return;
        }
    }
    // 移動入力がなければ待機ステートへ遷移
    else
    {
        //moveSpeed = defaultMoveSpeed; // 移動速度をリセット(待機ステートでリセットするか決める)
        TransitionIdleState();
        return;
    }
}


// ジャンプステートへ遷移
void Player::TransitionJumpState()
{
    state = State::Jump;
    
    // ジャンプ開始アニメーション再生
    PlayAnimation(Anim_JumpInit, false);
}

// ジャンプステート更新処理
void Player::UpdateJumpState(const float& elapsedTime)
{
    // ジャンプ開始アニメーションが終了したらジャンプアニメーション再生
    if (!IsPlayAnimation()) PlayAnimation(Anim_Jump, true);

    // 移動入力処理
    InputMove(elapsedTime);

    // ジャンプ入力処理
    InputJump();

    // ジャンプタイマーがある場合（ジャンプ中）
    if (jumpTimer > 0.0f)
    {
        // ジャンプボタンを押し続けると高く飛ぶ
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButton() & GamePad::BTN_B)
        {
            Jump(jumpSpeed);            // ジャンプ処理
            jumpTimer -= elapsedTime;   // ジャンプタイマー減算
        }
        // ジャンプボタンを離したらジャンプ終了
        else
        {
            jumpTimer = 0.0f;               // ジャンタイマーリセット
            PlayAnimation(Anim_Fall, true); // 落下アニメーション再生
        }
    }

    // 下方向に押されていたらヒップドロップステートへ遷移
    if (GetMoveVecY() < 0.0f)
    {
        TransitionHipDropState();
        return;
    }
}


// ヒップドロップステートへ遷移
void Player::TransitionHipDropState()
{
    state = State::HipDrop;

    // プレイヤーの正規化移動Xベクトルを求める(ステート遷移時に計算する)
    const float length = sqrtf(saveMoveVecX * saveMoveVecX);
    saveMoveVec_n      = saveMoveVecX / length;

    gravity  = hipDropGravity;  // 落下速度を上昇
    isBounce = true;            // バウンスさせる

    PlayAnimation(Anim_HipDrop, true);
}

// ヒップドロップステート更新処理
void Player::UpdateHipDropState(const float& elapsedTime)
{
    // 旋回処理（カメラ目線のままバウンスしないように更新する）
    Turn(elapsedTime, saveMoveVecX, turnSpeed);

    // 一回バウンスしたら重力をもとに戻す
    if (bounceCount != 0) gravity = defaultGravity;

    // バウンスが終了したら待機ステートへ遷移
    if (!isBounce)
    {
        TransitionIdleState();
        return;
    }

}
