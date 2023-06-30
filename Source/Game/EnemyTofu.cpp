#include "EnemyTofu.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"

int EnemyTofu::nameNum = 0;

// コンストラクタ
EnemyTofu::EnemyTofu()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/tohuall.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhuwalk.fbx", true);

    // ステートマシン
    stateMachine.reset(new StateMachine);

    GetStateMachine()->RegisterState(new TOFU::WalkState(this));
    GetStateMachine()->RegisterState(new TOFU::TurnState(this));
    GetStateMachine()->RegisterState(new TOFU::FindState(this));
    GetStateMachine()->RegisterState(new TOFU::TrackState(this));

    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Walk));


    // imgui名前かぶりが起きないように...
    name = "EnemyTofu" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// デストラクタ
EnemyTofu::~EnemyTofu()
{
}

// 初期化
void EnemyTofu::Initialize()
{
    health = 1;                                             // 体力設定
  
    moveRangeCenterX_ = GetTransform()->GetPosition().x; // 現在のX位置を移動範囲の中心に設定

    turnSpeed_ = ToRadian(3.0f);    // 旋回速度設定
}

// 終了化
void EnemyTofu::Finalize()
{
}

// Updateの前に呼び出される
void EnemyTofu::Begin()
{
}

// 更新処理
void EnemyTofu::Update(const float& elapsedTime)
{
    // ステート更新
    if (stateMachine) GetStateMachine()->Update(elapsedTime);

    UpdateAABB();                       // AABBの更新処理

    UpdateVelocity(elapsedTime);        // 速力処理更新処理

    CollisionEnemyVsPlayer();           // プレイヤーとの衝突判定処理
 
    UpdateInvincibleTimer(elapsedTime); // 無敵時間更新
    
    UpdateAnimation(elapsedTime);       // アニメーション更新
}

// Updateの後に呼び出される
void EnemyTofu::End()
{
}

// 描画処理
void EnemyTofu::Render(const float& elapsedTime)
{
    Enemy::Render(elapsedTime);
}

// debug用
void EnemyTofu::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    GetTransform()->DrawDebug();

    if (stateMachine)GetStateMachine()->DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}


void EnemyTofu::OnDead()
{
    // 自分を消去
    Destroy();
}
