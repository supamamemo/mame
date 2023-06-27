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
void EnemyTofu::Update(float elapsedTime)
{
    // ステート更新
    if (stateMachine)GetStateMachine()->Update(elapsedTime);
}

// Updateの後に呼び出される
void EnemyTofu::End()
{
}

// 描画処理
void EnemyTofu::Render(float elapsedTime)
{
    Character::Render(elapsedTime);

    //Graphics& graphics = Graphics::Instance();

    //// world行列更新
    //NO_CONST DirectX::XMFLOAT4X4 transform = {};
    //DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    //// model描画
    //if (&model->keyframe)
    //{
    //    model->skinned_meshes.render(graphics.GetDeviceContext(), transform, materialColor, &model->keyframe);
    //}
    //else
    //{
    //    model->skinned_meshes.render(graphics.GetDeviceContext(), transform, materialColor, nullptr);
    //}
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