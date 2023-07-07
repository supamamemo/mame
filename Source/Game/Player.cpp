#include "Player.h"

#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"
#include "../Mame/Scene/SceneManager.h"

#include "OperatorXMFLOAT3.h"
#include "EnemyManager.h"

#include "UIManager.h"

Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // モデル生成
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

    const DirectX::XMFLOAT3 min = { -0.4f, -0.0f, -0.4f }; // 符号ミスに注意
    const DirectX::XMFLOAT3 max = { +0.4f, +1.0f, +0.4f };
    SetAABB(min, max);
    UpdateAABB();

    // 待機ステートへ遷移
    TransitionIdleState();
}


Player::~Player()
{
}


void Player::Initialize()
{
    // ※ここの初期回転値によって振り向きの方向が変わるので注意
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(180), 0, 0));
}


void Player::Finalize()
{
}


void Player::Begin()
{
    
}

void Player::UpdateSelectStage(const float& elapsedTime)
{
    // AABB更新処理
    UpdateAABB();

    // アニメーション更新
    UpdateAnimation(elapsedTime);
}

void Player::Update(const float& elapsedTime)
{  
    Transform* transform = GetTransform();
    // 地形を保存していたら
    if (saveTerrain_)
    {
        // プレイヤーが地形の両端を超えていたら保存した地形情報を消去
        if ((aabb_.max.x < saveTerrain_->aabb_.min.x) ||
            (aabb_.min.x > saveTerrain_->aabb_.max.x))
        {
            saveTerrain_ = nullptr;
        }
        else
        {
            transform->SetPositionY(saveTerrain_->aabb_.max.y); // 地形の頭上をプレイヤーのY位置に代入
        }
    }

    // ステート分岐処理
    switch (state)
    {
    case State::Idle:    UpdateIdleState(elapsedTime);    break;
    case State::Walk:    UpdateWalkState(elapsedTime);    break;
    case State::Dash:    UpdateDashState(elapsedTime);    break;
    case State::Run:     UpdateRunState(elapsedTime);     break;
    case State::Jump:    UpdateJumpState(elapsedTime);    break;
    case State::HipDrop: UpdateHipDropState(elapsedTime); break;
    case State::Death:   UpdateDeathState(elapsedTime);   break;
    }

    // AABB更新処理
    UpdateAABB();

    // 速力更新処理
    UpdateVelocity(elapsedTime);

    // プレイヤーと敵の衝突判定
    CollisionPlayerVsEnemies();

    // 無敵時間中のキャラクターの点滅
    if (invincibleTimer > 0.0f)
    {
        //modelColor.y = 0.6f;
        //modelColor.x = (static_cast<int>(invincibleTimer * 100.0f) & 0x08) ? 1.0f : 0.0f;
        //modelColor.z = (static_cast<int>(invincibleTimer * 100.0f) & 0x08) ? 1.0f : 0.0f;

        modelColor.w = (static_cast<int>(invincibleTimer * 100.0f) & 0x08) ? 1.0f : 0.1f;
    }
    else
    {
        //modelColor.x = 1.0f;
        //modelColor.y = 1.0f;
        //modelColor.z = 1.0f;

        modelColor.w = 1.0f;
    }

    // 無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    // バウンス無敵時間更新
    UpdateBounceInvincibleTimer(elapsedTime);

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
    // 共通の描画処理
    Character::Render(elapsedTime);
}


// デバッグ描画
void Player::DrawDebug()
{
    ImGui::Begin("player");

    Character::DrawDebug();

    ImGui::Checkbox("isDebugBlendAnimation", &model->isDebugBlendAnimation);

    NO_CONST int animationIndex = model->GetCurrentAnimationIndex();
    ImGui::SliderInt("animationIndex", &animationIndex, 0, Anim_Max - 1);
    model->SetCurrentAnimationIndex(animationIndex);

    //ImGui::DragFloat2("box2dLenght", &box2d.lenght.x);

    ImGui::DragInt("hp", &health);

    // 速度関連パラメータ
    if (ImGui::TreeNode("Speed"))
    {
        ImGui::InputFloat3("velocity",         &velocity.x);                      // 速度(確認用)

        ImGui::SliderFloat("acceleration",     &acceleration_,     0.0f, 5.0f);    // 加速力
        ImGui::SliderFloat("d_Gravity",        &defaultGravity,   0.0f, 5.0f);    // 重力(バウンス後に反映)
        ImGui::SliderFloat("hipDropGravity",   &hipDropGravity,   1.0f, -10.0f);  // ヒップドロップ時の重力
        ImGui::SliderFloat("friction",         &friction_,         0.0f, 5.0f);    // 摩擦力（停止する力）
        ImGui::SliderFloat("airControl",       &airControl,       0.0f, 1.0f);    // 空中での身動きのしやすさ

        ImGui::SliderFloat("dashAcceleration", &dashAcceleration, 0.0f, 60.0f);   // ダッシュ時の加速力
        ImGui::SliderFloat("d_MoveSpeed",      &defaultMoveSpeed, 0.0f, 20.0f);   // 通常時の最高移動速度
        ImGui::SliderFloat("runMoveSpeed",     &runMoveSpeed,     0.0f, 30.0f);   // 走行時の最高移動速度

        ImGui::SliderFloat("turnSpeed_",       &turnSpeed_, 1.0f, ToRadian(900.0f), "%.0f");  // 旋回速度

        ImGui::TreePop();
    }

    // ジャンプ関連パラメータ
    if (ImGui::TreeNode("Jump"))
    {
        ImGui::SliderFloat("jumpSpeed_",  &jumpSpeed_,       0.0f, 30.0f);   // ジャンプ速度
        ImGui::SliderFloat("d_JumpTime", &defaultJumpTime, 0.0f, 1.0f);    // ジャンプし続けられる時間
        ImGui::SliderInt("jumpLimit",    &jumpLimit,       1,    5);       // ジャンプ最大回数

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
    Move(moveVecX, moveSpeed_);

    // 移動ベクトルがゼロベクトルじゃなければ（更新されていたら）保存する
    // ※保存することでボタンを押し続けなくても自動的に旋回しきるようになる
    if (moveVecX != 0.0f && moveVecX != saveMoveVecX_)
    {
        saveMoveVecX_ = moveVecX;
    }

    // 旋回処理(急ブレーキアニメーション再生中は処理をしない)
    if (model->GetCurrentAnimationIndex() != Anim_Break)
    {
        Turn(elapsedTime, saveMoveVecX_, turnSpeed_);
    }

    // 進行ベクトルがゼロベクトルでない場合は入力された
    return (moveVecX != 0.0f);
}


// ジャンプ入力
const bool Player::InputJump()
{
    // ボタン入力でジャンプ（ジャンプ回数制限付き）
    const GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & (GamePad::BTN_A | GamePad::BTN_B))
    {
        // 地面にいない場合はジャンプしない
        if (!isGround_) return false;

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


bool Player::ApplyDamage(const int& damage, const float& invincibleTime)
{
    // ダメージが0の場合は健康状態を変更する必要がない
    if (damage <= 0) return false;

    // 死亡している場合は健康状態を変更しない
    if (health <= 0) return false;

    // 無敵時間が残っていたら健康状態を変更しない
    if (invincibleTimer > 0.0f) return false;

    //　バウンス無敵時間が残っていたら健康状態を変更しない
    if (bounceInvincibleTimer_ > 0.0f) return false;

    // 無敵モードなら健康状態を変更しない
    if (isInvincible) return false;

    // ダメージ処理
    const int damagedHealth = health - damage;
    health = (damagedHealth > 0) ? damagedHealth : 0;

    // 無敵時間を設定
    invincibleTimer = invincibleTime;

    // ui
    UIManager::Instance().SetUiCenter(true);

    // 死亡通知
    if (health <= 0) OnDead();
    // ダメージ通知
    else OnDamaged();

    // 健康状態が変更した場合はtrueを返す
    return true;
}


void Player::CollisionPlayerVsEnemies()
{
    //if (invincibleTimer > 0.0f) return;

    // 死んでいるときは処理を行わない
    if (health <= 0) return;

    // バウンス時でなければ処理を行わない
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

            // ダメージを受けなければreturn
            if (!enemy->ApplyDamage(1, 1.0f)) return;

            // プレイヤーの移動方向保存ベクトルを取得
            const float length = fabsf(saveMoveVecX_);
            const float saveMoveVecX_n = (length > 0.0f) ? (saveMoveVecX_ / length) : 1.0f; // 単位ベクトル化(ゼロベクトルの予防込み)

            enemy->SetMoveDirectionX(saveMoveVecX_n); // プレイヤーの攻撃方向に吹っ飛ぶようにする

            Camera::Instance().PlayShake(ShakeType::VerticalShake);
        }
    }
}


void Player::UpdateDashCoolTimer(const float& elapsedTime)
{
    // クールタイムがなければ飛ばす
    if (dashCoolTimer <= 0.0f) return;

    // クールタイム減少
    dashCoolTimer -= elapsedTime;
}


// バウンス無敵時間更新処理
void Player::UpdateBounceInvincibleTimer(const float& elapsedTime)
{
    //　バウンス無敵時間がなければ飛ばす
    if (bounceInvincibleTimer_ <= 0.0f) return;

    // バウンス無敵時間を減らす
    bounceInvincibleTimer_ -= elapsedTime;
}


// 着地したときに呼ばれる関数
void Player::OnLanding()
{
    // ジャンプ回数リセット
    jumpCount = 0;

    // 移動速度が走行移動速度と同じ(走行状態)なら走行ステートへ遷移
    if (moveSpeed_ == runMoveSpeed)
    {
        TransitionRunState();
        return;
    }
    // 待機ステートへ遷移
    else
    {
        // 着地アニメーション再生
        PlayAnimation(Anim_JumpEnd, false, 0.5f, 0.5f);

        TransitionIdleState();
        return;
    }
}


// ダッシュ時に呼ばれる関数
void Player::OnDash()
{
    // ダッシュ時の速度を設定
    velocity.x = saveMoveVecX_ * dashAcceleration;
}


// バウンス時に呼ばれる関数
void Player::OnBounce()
{
    ++bounceCount; // バウンスカウント加算

    // 最大バウンスカウント以上ならバウンス終了させる
    if (bounceCount > bounceLimit)
    {
        velocity.y      = 0.0f;                 // Y速度リセット
        bounceSpeedX    = defaultBounceSpeedX;  // バウンスX速度リセット
        bounceSpeedY    = defaultBounceSpeedY;  // バウンスY速度リセット
        bounceCount     = 0;                    // バウンスカウントリセット
        isBounce        = false;                // バウンス終了
     
        OnLanding();                // 着地時の処理を行う
        isGround_        = true;    // 着地した
    }
    // バウンスさせる
    else
    {    
        velocity.x    = (saveMoveVecX_ > 0.0f) ? bounceSpeedX : -bounceSpeedX; // プレイヤーの向いている方向にバウンスX速度を代入    
        velocity.y    = bounceSpeedY;   // バウンスY速度を代入
        bounceSpeedX *= bounceScaleX;   // バウンスX速度を減少
        bounceSpeedY *= bounceScaleY;   // バウンスY速度を減少
    }
}


// ダメージを受けた時に呼ばれる関数
void Player::OnDamaged()
{
    // ヒットストップ再生
    Mame::Scene::SceneManager::Instance().PlayHitStop();

    SetModelColorAlpha(1.0f); // ヒットストップ時に無敵タイマーで透明にならないようにする
}


// 死亡したときに呼ばれる関数
void Player::OnDead()
{
    // ヒットストップ再生
    Mame::Scene::SceneManager::Instance().PlayHitStop();

    SetModelColorAlpha(1.0f); // ヒットストップ時に無敵タイマーで透明にならないようにする

    // 死亡ステートへ遷移
    TransitionDeathState();
}


// 落下死・落下ミスしたときに呼ばれる
void Player::OnFallDead()
{
    health -= 1; // 体力減少

    // 死んでいたらreturn
    if (health <= 0) return;

    // 最後に着地した地形の端っこに戻す
    {
        // 落ちた位置によって地形の左右どちらの端に復活するか決める
        if (aabb_.min.x <= lastLandingTerrainAABB_.min.x)
        {
            GetTransform()->SetPositionX(lastLandingTerrainAABB_.min.x + fabsf((aabb_.max.x - aabb_.min.x)));
        }
        else if (aabb_.max.x >= lastLandingTerrainAABB_.max.x)
        {
            GetTransform()->SetPositionX(lastLandingTerrainAABB_.max.x - fabsf((aabb_.max.x - aabb_.min.x)));
        }

        // 地形の頭上をY位置に設定
        GetTransform()->SetPositionY(lastLandingTerrainAABB_.max.y);

        UpdateAABB();           // 忘れずにAABB更新
    }

    invincibleTimer  =  1.0f;   // 無敵時間設定

    Camera::Instance().GetTransform()->SetPositionX(GetTransform()->GetPosition().x);
    Camera::Instance().GetTransform()->SetPositionY(GetTransform()->GetPosition().y);
     // ui
    UIManager::Instance().SetUiCenter(true);

    // 走行中・ジャンプ中・バウンス中に落ちたときのためにリセットする
    {
        moveSpeed_      =   defaultMoveSpeed;       // 移動速度リセット
        acceleration_    =   defaultAcceleration;    // 加速力リセット
        friction_        =   defaultFriction;        // 摩擦力リセット

        velocity.x      =   0.0f;                   // X速度リセット
        velocity.y      =   0.0f;                   // Y速度リセット
        bounceSpeedX    =   defaultBounceSpeedX;    // バウンスX速度リセット
        bounceSpeedY    =   defaultBounceSpeedY;    // バウンスY速度リセット
        bounceCount     =   0;                      // バウンスカウントリセット
        isBounce        =   false;                  // バウンス終了
        isInvincible    =   false;                  // 無敵モード解除

        gravity         =   defaultGravity;         // 重力リセット

        OnLanding();                                // 着地時の処理を行う
        isGround_        =  true;                   // 着地した
    }

    TransitionIdleState();      // 待機ステートへ遷移
}


// 待機ステートへ遷移
void Player::TransitionIdleState()
{
    state = State::Idle;

    // 着地アニメーションが再生されていなければ待機アニメーション再生
    if (model->GetCurrentAnimationIndex() != Anim_JumpEnd) PlayAnimation(Anim_Idle, true);
}

// 待機ステート更新処理
void Player::UpdateIdleState(const float& elapsedTime)
{
    // 着地アニメーションが終了したら待機アニメーション再生（ループしない着地アニメーションのときのみ処理が行われる）
    if (!IsPlayAnimation())
    {
        PlayAnimation(Anim_Idle, true);
    }

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
        if (gamePad.GetButtonDown() & (GamePad::BTN_X | GamePad::BTN_Y))
        {
            moveSpeed_       = defaultMoveSpeed; // 移動速度をリセット
            TransitionDashState();
            return;
        }
        // 移動速度が走行速度と同じ（走行状態）でダッシュキーが押され続けていれば走行ステートへ遷移
        else if (moveSpeed_ == runMoveSpeed && gamePad.GetButton() & (GamePad::BTN_X | GamePad::BTN_Y))
        {
            TransitionRunState();
            return;
        }
        // 移動入力だけなら歩行ステートへ遷移
        else
        {
            moveSpeed_       = defaultMoveSpeed; // 移動速度をリセット
            TransitionWalkState();
            return;
        }
    }

}


// 歩行ステートへ遷移
void Player::TransitionWalkState()
{
    state = State::Walk;

    PlayAnimation(Anim_Walk, true, 1.0f, 0.25f);
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
        if (gamePad.GetButtonDown() & (GamePad::BTN_X | GamePad::BTN_Y))
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

    PlayAnimation(Anim_Dash, false, 1.0f, 0.0f);
}

// ダッシュステート更新処理
void Player::UpdateDashState(const float& elapsedTime)
{

    // 旋回処理（カメラ目線のままダッシュしないように更新する）
    Turn(elapsedTime, saveMoveVecX_, turnSpeed_);

    
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
        if (gamePad.GetButton() & (GamePad::BTN_X | GamePad::BTN_Y))
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

    // 走行時の速度パラメータを設定
    moveSpeed_      = runMoveSpeed;
    acceleration_    = runAcceleration;
    friction_        = runFriction;

    // 走行用保存移動ベクトルに移動ベクトルを保存
    if (runMoveVecX == 0.0f) runMoveVecX = moveVecX_;

    // 走行アニメーション再生
    PlayAnimation(Anim_Run, true, 1.0f, 0.5f);
}

// 走行ステート更新処理
void Player::UpdateRunState(const float& elapsedTime)
{
    // ブレーキ・着地アニメーションが終わったら走行アニメーションに戻る
    if (!IsPlayAnimation()) 
    {
        PlayAnimation(Anim_Run, true, 1.0f, 0.5f);
    }

    // ジャンプ入力処理(ジャンプしていたらジャンプステートへ遷移)
    if (InputJump())
    {   
        acceleration_ = defaultAcceleration;
        friction_     = defaultFriction;

        runMoveVecX = moveVecX_; // 移動ベクトル保存を更新しておく（ダッシュジャンプして着地する寸前に方向転換するとカメラ目線になるため）

        TransitionJumpState();
        return;
    }
    else if (InputMove(elapsedTime))
    {
        // 速度が最高速度に達していて、
        // 走行用保存移動ベクトルと現在の移動ベクトルの符号が違う場合（方向転換）はブレーキアニメーションを再生
        const bool isPlayBreakAnimation = {
            (velocity.x >=  runMoveSpeed * 0.8f && runMoveVecX > 0.0f && moveVecX_ < 0.0f) ||
            (velocity.x <= -runMoveSpeed * 0.8f && runMoveVecX < 0.0f && moveVecX_ > 0.0f)
        };
        if (isPlayBreakAnimation)
        {
            PlayAnimation(Anim_Break, false, 1.8f, 0.5f);
        }

        // 走行キー入力がなければ歩行ステートへ
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (!(gamePad.GetButton() & (GamePad::BTN_X | GamePad::BTN_Y)))
        {
            // 速度パラメータをリセット
            moveSpeed_    = defaultMoveSpeed;
            acceleration_ = defaultAcceleration;
            friction_     = defaultFriction;
            TransitionWalkState();
            return;
        }

        runMoveVecX = moveVecX_; // 保存移動ベクトル更新
    }
    // 移動入力がなければ待機ステートへ遷移
    else
    {
        //moveSpeed = defaultMoveSpeed; // 移動速度をリセット(待機ステートでリセットするか決める)
        acceleration_ = defaultAcceleration;
        friction_     = defaultFriction;
        TransitionIdleState();
        return;
    }
}


// ジャンプステートへ遷移
void Player::TransitionJumpState()
{
    state = State::Jump;

    // ジャンプした位置を保存
    jumpedPositionY = GetTransform()->GetPosition().y;
    
    // ジャンプ開始アニメーション再生
    PlayAnimation(Anim_JumpInit, false, 1.0f);
}

// ジャンプステート更新処理
void Player::UpdateJumpState(const float& elapsedTime)
{
    // 落下し始めたら落下アニメーション再生
    if (velocity.y < 0.0f)
    {
        PlayAnimation(Anim_Fall, true);
    }
    // ジャンプ開始アニメーションが終了したらジャンプアニメーション再生
    else if (!IsPlayAnimation())
    {
        PlayAnimation(Anim_Jump, true, 1.25f);
    }

    // 移動入力処理
    InputMove(elapsedTime);

    // ジャンプ入力処理
    InputJump();

    // ジャンプタイマーがある場合（ジャンプ中）
    if (jumpTimer > 0.0f)
    {
        // ジャンプボタンを押し続けると高く飛ぶ
        const GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButton() & (GamePad::BTN_A | GamePad::BTN_B))
        {
            Jump(jumpSpeed_);            // ジャンプ処理
            jumpTimer -= elapsedTime;   // ジャンプタイマー減算
        }
        // ジャンプボタンを離したらジャンプ終了
        else
        {
            jumpTimer = 0.0f;           // ジャンタイマーリセット
        }
    }

    // 下方向に押されていたら
    if (GetMoveVecY() < -0.9f)
    {      
        // ジャンプ開始時のY位置と現在のY位置からジャンプした高さを算出
        const float jumpHeight = fabsf(GetTransform()->GetPosition().y - jumpedPositionY);

        // ヒップドロップに必要な距離(高さ)に達していたらヒップドロップステートへ遷移
        if (jumpHeight >= needHipDropHeight)
        {
            TransitionHipDropState();
            return;
        }
    }
}


// ヒップドロップステートへ遷移
void Player::TransitionHipDropState()
{
    state = State::HipDrop;

    gravity         = hipDropGravity;  // 落下速度を上昇
    isBounce        = true;            // バウンスさせる

#if 0
    invincibleTimer        = 1.1f;            // 無敵時間を設定(確認用)
#else 
    bounceInvincibleTimer_ = 1.1f;            // バウンド無敵時間を設定
#endif

    PlayAnimation(Anim_HipDrop, true);
}

// ヒップドロップステート更新処理
void Player::UpdateHipDropState(const float& elapsedTime)
{
    // 旋回処理（カメラ目線のままバウンスしないように更新する）
    Turn(elapsedTime, saveMoveVecX_, turnSpeed_);

    // 一回バウンスしたら重力をもとに戻す
    if (bounceCount != 0) gravity = defaultGravity;

    //// バウンスが終了したら待機ステートへ遷移
    //if (!isBounce)
    //{
    //    TransitionIdleState();
    //    return;
    //}

}


void Player::TransitionDeathState()
{
    state = State::Death;

    // 速度リセット
    velocity = {};

    // 正面を向くように設定
    GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(-5.0f), ToRadian(180.0f), 0, 0));
    
    Jump(jumpSpeed_ * 2.0f);    // 飛び上がらせる

    gravity = deathGravity_;    // 重力を軽くする

    invincibleTimer = 0.0f;     // 点滅させないように無敵タイマーをリセットする

    // ダッシュアニメ―ションを再生
    PlayAnimation(Anim_Dash, true, 1.25f);
}

void Player::UpdateDeathState(const float& elapsedTime)
{
    // 画面に向かって来る
    GetTransform()->AddPositionZ(-15.0f * elapsedTime);
}