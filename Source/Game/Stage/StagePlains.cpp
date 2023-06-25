#include "StagePlains.h"

#include "../../Mame/Graphics/Camera.h"
#include "../EnemyManager.h"
#include "../EnemyCannon.h"

// コンストラクタ
StagePlains::StagePlains()
{
    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // エネミー初期化
    //EnemyCannon* cannon = new EnemyCannon();
    //cannon->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 20.0f));
    //EnemyManager::Instance().Register(cannon);

    EnemyCannon* cannon2 = new EnemyCannon();
    cannon2->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
    EnemyManager::Instance().Register(cannon2);

    // 草ブロック生成
#if 0
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block = std::make_unique<GrassBlock>();
    }
#else
    //grassBlock[0] = std::make_unique<GrassBlock>("./resources/temporary/assets_air_ground.fbx");
    //grassBlock[1] = std::make_unique<GrassBlock>("./resources/temporary/assets_air_ground_move.fbx");
    //grassBlock[2] = std::make_unique<GrassBlock>("./resources/temporary/assets_block.fbx");
    //grassBlock[3] = std::make_unique<GrassBlock>("./resources/temporary/assets_cannon.fbx");
    //grassBlock[4] = std::make_unique<GrassBlock>("./resources/temporary/assets_cannon_bullet.fbx");
    //grassBlock[5] = std::make_unique<GrassBlock>("./resources/temporary/assets_flag.fbx");
    //grassBlock[0] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    //grassBlock[1] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    //grassBlock[2] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    //grassBlock[3] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    //grassBlock[4] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    //grassBlock[5] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    //grassBlock[6] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");

    grassBlock[0] = std::make_unique<GrassBlock>("./resources/stage/1.fbx");
    grassBlock[1] = std::make_unique<GrassBlock>("./resources/stage/2.fbx");
    grassBlock[2] = std::make_unique<GrassBlock>("./resources/stage/3.fbx");
    grassBlock[3] = std::make_unique<GrassBlock>("./resources/stage/4.fbx");
    grassBlock[4] = std::make_unique<GrassBlock>("./resources/stage/5.fbx");
    grassBlock[5] = std::make_unique<GrassBlock>("./resources/stage/6.fbx");
    grassBlock[6] = std::make_unique<GrassBlock>("./resources/stage/7.fbx");
    grassBlock[7] = std::make_unique<GrassBlock>("./resources/stage/8.fbx");
    grassBlock[8] = std::make_unique<GrassBlock>("./resources/stage/9.fbx");
    grassBlock[9] = std::make_unique<GrassBlock>("./resources/stage/10.fbx");
    grassBlock[10] = std::make_unique<GrassBlock>("./resources/stage/11.fbx");
    grassBlock[11] = std::make_unique<GrassBlock>("./resources/stage/12.fbx");
    grassBlock[12] = std::make_unique<GrassBlock>("./resources/stage/13.fbx");
    grassBlock[13] = std::make_unique<GrassBlock>("./resources/stage/14.fbx");
    grassBlock[14] = std::make_unique<GrassBlock>("./resources/stage/15.fbx");
    grassBlock[15] = std::make_unique<GrassBlock>("./resources/stage/15.fbx");
    grassBlock[16] = std::make_unique<GrassBlock>("./resources/stage/16.fbx");
    grassBlock[17] = std::make_unique<GrassBlock>("./resources/stage/17.fbx");
    grassBlock[18] = std::make_unique<GrassBlock>("./resources/stage/18.fbx");
    grassBlock[19] = std::make_unique<GrassBlock>("./resources/stage/19.fbx");
    grassBlock[20] = std::make_unique<GrassBlock>("./resources/stage/20.fbx");
    grassBlock[21] = std::make_unique<GrassBlock>("./resources/stage/21.fbx");
    grassBlock[22] = std::make_unique<GrassBlock>("./resources/stage/15.fbx");
    grassBlock[23] = std::make_unique<GrassBlock>("./resources/stage/15.fbx");
    grassBlock[24] = std::make_unique<GrassBlock>("./resources/stage/22.fbx");
    grassBlock[25] = std::make_unique<GrassBlock>("./resources/stage/23.fbx");
    

#endif
}

// 初期化
void StagePlains::Initialize()
{
    // player初期化
    PlayerManager::Instance().GetPlayer()->Initialize();

    // 草ブロック初期化
    int temp = 0;
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {        
        block->Initialize(DirectX::XMFLOAT3(0,0,20));
        temp++;
    }


    // ここで地面を並べています
    {
        grassBlock[0].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
        grassBlock[1].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(7.0f, 1.5f, 10.0f));
        grassBlock[2].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(8.65f, 1.5f, 10.0f));
        grassBlock[3].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(18.0f, 0.0f, 10.0f));
        grassBlock[4].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(33.0f, 0.0f, 10.0f));
        grassBlock[5].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(33.0f, 1.5f, 10.0f));
        grassBlock[6].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(52.0f, 0.0f, 10.0f));
        grassBlock[7].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(58.0f, 0.0f, 10.0f));
        grassBlock[8].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(63.0f, 0.0f, 10.0f));
        grassBlock[9].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(68.0f, 0.0f, 10.0f));
        grassBlock[10].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(82.0f, 0.0f, 10.0f));
        grassBlock[11].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(86.0f, 1.8f, 10.0f));
        grassBlock[12].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(90.7f, 1.8f, 10.0f));
        grassBlock[13].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(95.0f, 0.0f, 10.0f));
        grassBlock[14].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(99.0f, 6.0f, 10.0f));
        grassBlock[15].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(102.0f, 7.0f, 10.0f));
        grassBlock[16].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(105.0f, 0.0f, 10.0f));
        grassBlock[17].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(116.0f, 0.0f, 10.0f));
        grassBlock[18].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(118.99f, 3.46f, 10.0f));
        grassBlock[19].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(121.25f, 3.46f, 10.0f));
        grassBlock[20].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(151.0f, 0.0f, 10.0f));
        grassBlock[21].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(131.06f, 1.6f, 10.0f));
        grassBlock[22].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(138.0f, 4.0f, 10.0f));
        grassBlock[23].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(141.0f, 5.0f, 10.0f));
        grassBlock[24].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(146.0f, 1.6f, 10.0f));
        grassBlock[25].get()->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(149.3f, 1.6f, 10.0f));
    }

    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 10, -12));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(10), 0, 0, 0));
}

// 終了化
void StagePlains::Finalize()
{
    // player終了化
    PlayerManager::Instance().GetPlayer()->Finalize();

    // エネミー終了化
    EnemyManager::Instance().Clear();

    // 草ブロック終了化
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Finalize();
    }
}

// Updateの前に呼ばれる処理
void StagePlains::Begin()
{
    // player
    PlayerManager::Instance().GetPlayer()->Begin();

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Begin();
    }
}

// 更新処理
void StagePlains::Update(const float& elapsedTime)
{
    // player更新
    PlayerManager::Instance().GetPlayer()->Update(elapsedTime);

    // エネミー更新
    EnemyManager::Instance().Update(elapsedTime);

    // 草ブロック更新
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Update();
    }

    Camera& camera = Camera::Instance();
    camera.Update(elapsedTime);

#if 0 // SORRY
    const Collision::Box3D box1 = {
        PlayerManager::Instance().GetPlayer()->model->skinned_meshes.boundingBox[0],
        PlayerManager::Instance().GetPlayer()->model->skinned_meshes.boundingBox[1]
    };
    const Collision::Box3D box2 = {
        grassBlock[0]->model->skinned_meshes.boundingBox[0],
        grassBlock[0]->model->skinned_meshes.boundingBox[1],
    };

    NO_CONST Collision::Box3D outPosition = {};

    if (Collision::IntersectBox3DVsBox3D(box1, box2, outPosition))
    {
        PlayerManager::Instance().GetPlayer()->model->skinned_meshes.boundingBox[0] = outPosition.min;
        PlayerManager::Instance().GetPlayer()->model->skinned_meshes.boundingBox[1] = outPosition.max;
    }
#endif
}

// Updateの後に呼ばれる処理
void StagePlains::End()
{
    // player
    PlayerManager::Instance().GetPlayer()->End();

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->End();
    }
}

// 描画処理
void StagePlains::Render(const float& elapsedTime)
{
    // player
    PlayerManager::Instance().GetPlayer()->Render(elapsedTime);

    // エネミー描画
    EnemyManager::Instance().Render(elapsedTime);

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        // DrawCollを少なくするためにplayerから近いものだけ描画する
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 blockPos = block.get()->model->GetTransform()->GetPosition();

        DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&blockPos));
        float vecX = DirectX::XMVectorGetX(vec);
        vecX = (vecX > 0) ? vecX : -vecX;

        if (vecX < 40.0f)block->Render(elapsedTime);
    }
}

// debug用
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    PlayerManager::Instance().GetPlayer()->DrawDebug();

    // エネミー
    EnemyManager::Instance().DrawDebug();

    // 草ブロック
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->DrawDebug();
    }

#endif
}
