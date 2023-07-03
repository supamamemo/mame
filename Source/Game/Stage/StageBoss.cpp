#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"

#include "../EnemyManager.h"

#include "../Terrain/TerrainBoss.h"
#include "../Terrain/TerrainManager.h"

// コンストラクタ
StageBoss::StageBoss()
{
    Graphics& graphics = Graphics::Instance();

    // ステージ生成&登録
    {
        //Terrain* terrain0 = new TerrainBoss("./resources/stage/stage.fbx");
        Terrain* terrain0 = new TerrainBoss("./resources/stage/1.fbx");
        TerrainManager::Instance().Register(terrain0);

        Terrain* terrain1 = new TerrainBoss("./resources/stage/1.fbx");
        TerrainManager::Instance().Register(terrain1);

        Terrain* terrain2 = new TerrainBoss("./resources/stage/1.fbx");
        TerrainManager::Instance().Register(terrain2);

        Terrain* terrain3 = new TerrainBoss("./resources/stage/1.fbx");
        TerrainManager::Instance().Register(terrain3);        
    }


    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // boss生成
    EnemyManager::Instance().Register(new Boss());

    // bossのhp用
    chefHat = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/chefHat.png");

    // 背景仮
    back = std::make_unique<Boss>("./resources/back.fbx");

    // tofu
    //EnemyTofu* tofu = new EnemyTofu();
    //tofu->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 1.5f, 10.0f));
    //tofu->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(90), 0.0f, 0.0f));
    //EnemyManager::Instance().Register(tofu);
    
}

// 初期化
void StageBoss::Initialize()
{
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0.0f, 0.0f, 0.0f));


    // 背景仮
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 2.0f, 32.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 17.0f, 8.0f));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0.0f, 0.0f));

    // ステージ初期設定
    {
        TerrainManager& terrainManager = TerrainManager::Instance();

        // 位置設定
        {
            terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 10));
            terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-9, 21, 10));
            terrainManager.GetTerrain(1)->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 5, 1));
            terrainManager.GetTerrain(1)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0));
            terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(16.5f, 21, 10));
            terrainManager.GetTerrain(2)->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 5, 1));

            terrainManager.GetTerrain(2)->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0));
            terrainManager.GetTerrain(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 10.0f, 10));
            terrainManager.GetTerrain(3)->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 2.0f, 1.0f));
        }
        
        // materialColor
        terrainManager.GetTerrain(0)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));
        terrainManager.GetTerrain(1)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));
        terrainManager.GetTerrain(2)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));
        terrainManager.GetTerrain(3)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));

        terrainManager.Initialize();
    }

    // player初期化
    PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 1.0f, 10.0f));
    PlayerManager::Instance().Initialize();

    // boss初期化   
    EnemyManager::Instance().Initialize();
}

// 終了化
void StageBoss::Finalize()
{
    // boss終了化
    //boss->Finalize();
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear(); // vectorクリア

    // player終了化
    PlayerManager::Instance().Finalize();

    // terrain終了化
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear(); // vectorクリア
}

// Updateの前に呼ばれる
void StageBoss::Begin()
{
    // terrain
    TerrainManager::Instance().Begin();

    // player
    PlayerManager::Instance().Begin();

    // boss
    //boss->Begin();
    EnemyManager::Instance().Begin();
}

// 更新処理
void StageBoss::Update(const float& elapsedTime)
{
    Camera::Instance().UpdateBoss(elapsedTime);

    // terrain更新
    TerrainManager::Instance().Update(elapsedTime);

    // player更新
    PlayerManager& playerManager = PlayerManager::Instance();
    playerManager.Update(elapsedTime);
    
    // enemy更新
    EnemyManager::Instance().Update(elapsedTime);
}

// Updateの後に呼ばれる処理
void StageBoss::End()
{
    // terrain
    TerrainManager::Instance().End();

    // player
    PlayerManager::Instance().End();

    // boss
    //boss->End();
    EnemyManager::Instance().End();
}

// 描画処理
void StageBoss::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // terrain
    TerrainManager::Instance().Render(elapsedTime);

    // player描画
    PlayerManager::Instance().Render(elapsedTime);

    // boss描画
    EnemyManager::Instance().Render(elapsedTime);

    // 背景仮
    back->Render(elapsedTime);

    // tofu

    //tofu->Render(elapsedTime);

    // bossHp
    shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);
    chefHat->render(graphics.GetDeviceContext(), spr.pos.x, spr.pos.y, spr.texPos.x, spr.texPos.y);

}

// debug用
void StageBoss::DrawDebug()
{
#ifdef USE_IMGUI

    // terrain
    TerrainManager::Instance().DrawDebug();
 
    // player
    PlayerManager::Instance().DrawDebug();

    // boss
    EnemyManager::Instance().DrawDebug();

    // bossHp
    

    // 背景仮
    back->DrawDebug();


    ImGui::Begin("spr");
    ImGui::DragFloat2("pos", &spr.pos.x);
    ImGui::DragFloat2("texPos", &spr.texPos.x);
    ImGui::End();

#endif
}
