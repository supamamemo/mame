#include "EnemyCannon.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"

int EnemyCannon::nameNum = 0;

// コンストラクタ
EnemyCannon::EnemyCannon()
{
    Graphics& graphics = Graphics::Instance();

    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhu.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_cannon.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_flag.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/matome0622.fbx", true);

    // ステートマシンをセット
    stateMachine.reset(new StateMachine);

    GetStateMachine()->RegisterState(new CANNON::IdleState(this));
    GetStateMachine()->RegisterState(new CANNON::AttackState(this));

    GetStateMachine()->SetState(static_cast<int>(CANNON::STATE::Idle));


    // imgui名前かぶりが起きないように...
    name = "EnemyCannon" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// デストラクタ
EnemyCannon::~EnemyCannon()
{
}

// 初期化
void EnemyCannon::Initialize()
{
}

// 終了化
void EnemyCannon::Finalize()
{
}

// Updateの前に呼ばれる
void EnemyCannon::Begin()
{
}

// 更新処理
void EnemyCannon::Update(float elapsedTime)
{
    // cannonBallManager
    cannonBallManager.Update(elapsedTime);

    if (stateMachine) GetStateMachine()->Update(elapsedTime);

    // AABB更新処理
    UpdateAABB(elapsedTime);

    // デバッグモデルの位置更新
    debugModel->GetTransform()->SetPosition(model->GetTransform()->GetPosition());
}

// Updateの後に呼ばれる
void EnemyCannon::End()
{
}

// 描画処理
void EnemyCannon::Render(float elapsedTime)
{
    // 共通の描画処理
    Character::Render(elapsedTime);

    // cannonBallManager
    cannonBallManager.Render(elapsedTime);

}

// debug用
void EnemyCannon::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    model->GetTransform()->DrawDebug();

    if (stateMachine) GetStateMachine()->DrawDebug();

    cannonBallManager.DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}
