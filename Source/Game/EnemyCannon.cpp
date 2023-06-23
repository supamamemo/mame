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
    Graphics& graphics = Graphics::Instance();

    // Transform更新
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (&model->keyframe)
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, materialColor, &model->keyframe);
    }
    else
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
    }

#if _DEBUG
    // BOUNDING_BOX
    {
        DirectX::XMFLOAT4X4 debugTransform = {};

        // ワールド行列の取得とスケール調整
        DirectX::XMStoreFloat4x4(&debugTransform, debugModel->GetTransform()->CalcWorldMatrix(0.01f));

        // ワールド行列設定
        debugTransform = SetDebugModelTransform(debugTransform);

        // 描画
        debugModel->skinned_meshes.render(graphics.GetDeviceContext(), debugTransform, { 1.0f, 0.0f, 0.0f, 0.2f }, nullptr);
    }
#endif // _DEBUG

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
