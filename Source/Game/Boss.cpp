#include "Boss.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Input/Input.h"

#include "BossStateDerived.h"

int Boss::nameNum = 0;

// コンストラクタ
Boss::Boss()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_block.fbx", true);
    DirectX::XMFLOAT3 pos1 = model->GetTransform()->GetPosition();
    //aabb = { {pos1.x,pos1.y,pos1.z},{0.5f,0.5f,0.5f} };


    debugModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/cube.fbx", true);

    // ステートマシーンをセット
    stateMachine.reset(new StateMachine);

    GetStateMachine()->RegisterState(new BOSS::IdleState(this));
    GetStateMachine()->RegisterState(new BOSS::FindState(this));
    GetStateMachine()->RegisterState(new BOSS::AttackState(this));
    GetStateMachine()->RegisterState(new BOSS::RecoilState(this));

    GetStateMachine()->SetState(static_cast<int>(BOSS::STATE::Idle));


    // imgui名前かぶり起きないように...
    name = "Boss" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

Boss::Boss(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), filename, true);

    debugModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);

    // imgui名前かぶり起きないように...
    name = "Boss" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// デストラクタ
Boss::~Boss()
{
}

// 初期化
void Boss::Initialize()
{
    model->GetTransform()->SetPosition(DirectX::XMFLOAT3(2, 0, 10));
}

// 終了化
void Boss::Finalize()
{
}

// Updateの前に呼ばれる
void Boss::Begin()
{
}

// 更新処理
void Boss::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = gamePad.GetAxisRX();

    float speed = 0.001f;
    speed *= ax;
    DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    pos.x += speed;
    model->GetTransform()->SetPosition(pos);


    debugModel->GetTransform()->SetPosition(model->GetTransform()->GetPosition());
    debugModel->GetTransform()->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
    
    if (stateMachine)GetStateMachine()->Update(elapsedTime);
}

// Updateの後に呼ばれる
void Boss::End()
{
}

// 描画処理
void Boss::Render(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // Transform更新
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (&keyframe)
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, materialColor, &keyframe);
    }
    else
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, materialColor, nullptr);
    }

#if _DEBUG
    // BOUNDING_BOX
    {
        DirectX::XMFLOAT4X4 t = SetDebugModelTransform(transform);
        debugModel->skinned_meshes.render(graphics.GetDeviceContext(), t, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.2f), nullptr);
    }
#endif // _DEBUG
}

// debug用
void Boss::DrawDebug()
{
    ImGui::Begin(GetName());

    Character::DrawDebug();

    if (stateMachine)GetStateMachine()->DrawDebug();

    ImGui::End();
}