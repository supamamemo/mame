#include "StagePlains.h"

#include "../../Mame/Graphics/Graphics.h"
#include "../../Mame/Graphics/Camera.h"
#include "../../Mame/Scene/SceneManager.h"
#include "../EnemyManager.h"
#include "../EnemyTofu.h"
#include "../EnemyRedTofu.h"
#include "../Terrain/TerrainManager.h"
#include "../Terrain/TerrainPlains.h"

#include "../UIManager.h"

// コンストラクタ
StagePlains::StagePlains()
{
    // terrain生成
    TerrainManager& terrainManager = TerrainManager::Instance();
    {
        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx" )); // 0
        terrainManager.Register(new TerrainPlains("./resources/stage/2.fbx" )); // 1
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx" )); // 2
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx" )); // 3
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 4
        terrainManager.Register(new TerrainPlains("./resources/stage/6.fbx" )); // 5
        terrainManager.Register(new TerrainPlains("./resources/stage/6.fbx" )); // 6
        terrainManager.Register(new TerrainPlains("./resources/stage/8.fbx" )); // 7
        terrainManager.Register(new TerrainPlains("./resources/stage/8.fbx" )); // 8
        terrainManager.Register(new TerrainPlains("./resources/stage/2.fbx" )); // 9
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 10

        terrainManager.Register(new TerrainPlains("./resources/stage/2.fbx" )); // 11
        terrainManager.Register(new TerrainPlains("./resources/stage/2.fbx" )); // 12
        terrainManager.Register(new TerrainPlains("./resources/stage/7.fbx" )); // 13
        terrainManager.Register(new TerrainPlains("./resources/stage/7.fbx" )); // 14
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx" )); // 15
        
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 16
        terrainManager.Register(new TerrainPlains("./resources/stage/7.fbx" )); // 17

        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 18
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 19
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 20
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 21
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 22

        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 23
        terrainManager.Register(new TerrainPlains("./resources/stage/2.fbx" )); // 24
    }

    // player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // enemy生成
    EnemyManager& enemyManager = EnemyManager::Instance();
    {
        // 23体生成
        for (int i = 0; i < 23; ++i)
        {
            //enemyManager.Register(new EnemyTofu());
            enemyManager.Register(new RED_TOFU::EnemyRedTofu());
        }        
    }

    // UI
    {
        UIManager& uiManager = UIManager::Instance();

        uiManager.Register(new UI(L"./resources/ui/baseMameHp.png"));
        uiManager.Register(new UI(L"./resources/ui/mameLeft.png"));
        uiManager.Register(new UI(L"./resources/ui/mameCenter.png"));
        uiManager.Register(new UI(L"./resources/ui/mameRight.png"));
    }
}

// 初期化
void StagePlains::Initialize()
{
    // camera初期化
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 10, -12));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0, 0, 0));


    const float posZ = 10.0f; // 奥行き

    // terrain初期化
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        // ここで地面を並べています
        {
            terrainManager.GetTerrain(0 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f,     0.0f, posZ));
            terrainManager.GetTerrain(1 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(29.0f,    0.0f, posZ));
            terrainManager.GetTerrain(2 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(21.0f,   -1.2f, posZ));
            terrainManager.GetTerrain(3 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(26.0f,    0.0f, posZ));
            terrainManager.GetTerrain(4 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(34.0f,    0.0f, posZ));

            terrainManager.GetTerrain(5 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(41.5f,    1.5f, posZ));
            terrainManager.GetTerrain(6 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(54.0f,    1.5f, posZ));
            terrainManager.GetTerrain(7 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(62.0f,    6.0f, posZ));
            terrainManager.GetTerrain(8 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(67.0f,    4.0f, posZ));
            terrainManager.GetTerrain(9 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(78.0f,    0.0f, posZ));

            terrainManager.GetTerrain(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3(90.75f,  -1.0f, posZ));
            terrainManager.GetTerrain(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(102.0f,   0.0f, posZ));
            terrainManager.GetTerrain(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(124.7f,   0.0f, posZ));
            terrainManager.GetTerrain(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(101.5f,   7.0f, posZ));
            terrainManager.GetTerrain(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(113.455f, 8.0f, posZ));

            terrainManager.GetTerrain(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(85.55f,   0.0f, posZ));
            terrainManager.GetTerrain(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(113.35f, -3.0f, posZ));
            terrainManager.GetTerrain(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(125.0f,   7.0f, posZ));                                                                                          
            terrainManager.GetTerrain(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(136.0f,  -1.0f, posZ));
            terrainManager.GetTerrain(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(143.0f,   1.0f, posZ));

            terrainManager.GetTerrain(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(150.0f,   3.0f, posZ));
            terrainManager.GetTerrain(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(155.25f, -6.0f, posZ));
            terrainManager.GetTerrain(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(160.6f,  -5.0f, posZ));
            terrainManager.GetTerrain(23)->GetTransform()->SetPosition(DirectX::XMFLOAT3(168.0f,  -3.0f, posZ));
            terrainManager.GetTerrain(24)->GetTransform()->SetPosition(DirectX::XMFLOAT3(174.0f,  -1.0f, posZ));
        }
        terrainManager.Initialize();
    }

    // player初期化
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 1.5f, posZ));
        playerManager.Initialize();
    }

    // enemy初期化  
    {
        EnemyManager& enemyManager = EnemyManager::Instance();

        // 位置設定
        {
#if 0
            // 豆腐の上下の押し戻し処理確認用
            enemyManager.GetEnemy(0 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(12.0f,   1.5f,  posZ));
            enemyManager.GetEnemy(1 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(12.0f,   20.5f, posZ));
            enemyManager.GetEnemy(2 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(12.0f,   30.5f, posZ));
#else
            enemyManager.GetEnemy(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.0f,    1.5f,  posZ));
            enemyManager.GetEnemy(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(13.0f,    20.5f, posZ));
            enemyManager.GetEnemy(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(15.0f,    30.5f, posZ));
#endif
            enemyManager.GetEnemy(3 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(24.5f,   1.5f,  posZ));
            enemyManager.GetEnemy(4 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(33.0f,   3.0f,  posZ));
            enemyManager.GetEnemy(5 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(40.0f,   1.5f,  posZ));
            enemyManager.GetEnemy(6 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(43.0f,   1.5f,  posZ));
            enemyManager.GetEnemy(7 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(52.5f,   2.0f,  posZ));
            enemyManager.GetEnemy(8 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(58.0f,   4.0f,  posZ));
            enemyManager.GetEnemy(9 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(68.0f,   3.0f,  posZ));
            enemyManager.GetEnemy(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3(75.0f,   1.75f, posZ));
            enemyManager.GetEnemy(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(81.0f,   1.75f, posZ));
            enemyManager.GetEnemy(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(88.5f,   1.75f, posZ));
            enemyManager.GetEnemy(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(112.0f,  3.35f, posZ));
            enemyManager.GetEnemy(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(114.0f,  3.35f, posZ));
            enemyManager.GetEnemy(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(116.0f,  3.35f, posZ));
            enemyManager.GetEnemy(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(121.25f, 4.5f,  posZ));
            enemyManager.GetEnemy(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(131.0f,  2.5f,  posZ));
            enemyManager.GetEnemy(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(138.0f,  1.5f,  posZ));
            enemyManager.GetEnemy(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(141.0f,  1.5f,  posZ));
            enemyManager.GetEnemy(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(153.0f,  1.5f,  posZ));
            enemyManager.GetEnemy(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(156,     1.5f,  posZ));
            enemyManager.GetEnemy(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(159,     1.5f,  posZ));
        }

        enemyManager.Initialize();
    }

    // UI
    {
        UIManager::Instance().GetUI(UISPRITE::BaseMameHp)->SetPosition(playerUiPos);
        UIManager::Instance().GetUI(UISPRITE::BaseMameHp)->SetSize(playerUiSize);
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetPosition(playerUiPos);
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetSize(playerUiSize);
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetPosition(playerUiPos);
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetSize(playerUiSize);
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetPosition(playerUiPos);
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetSize(playerUiSize);

        UIManager::Instance().GetUI(UISPRITE::BaseMameHp)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetIsRender(true);
    }
    
}

// 終了化
void StagePlains::Finalize()
{
    // エネミー終了化
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear();

    // player終了化
    PlayerManager::Instance().Finalize();

    // terrain終了化
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear();

    // uimanager
    UIManager::Instance().Finalize();
    UIManager::Instance().Clear();
}

// Updateの前に呼ばれる処理
void StagePlains::Begin()
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        // player
        PlayerManager::Instance().Begin();

        // enemy
        EnemyManager::Instance().Begin();

        // terrain
        TerrainManager::Instance().Begin();
    }
}

// 更新処理
void StagePlains::Update(const float& elapsedTime)
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        // camera更新
        Camera::Instance().Update(elapsedTime);

        // player更新
        PlayerManager::Instance().Update(elapsedTime);

        // enemy更新
        EnemyManager::Instance().Update(elapsedTime);

        // terrain更新
        TerrainManager::Instance().Update(elapsedTime);
    }

    // UI
    UIManager::Instance().Update(elapsedTime);
    PlayerHpUiUpdate(elapsedTime);
}

// Updateの後に呼ばれる処理
void StagePlains::End()
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        // player
        PlayerManager::Instance().End();

        // enemy
        EnemyManager::Instance().End();

        // terrain
        TerrainManager::Instance().End();
    }
}

// 描画処理
void StagePlains::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // player描画
    PlayerManager::Instance().Render(elapsedTime);

    // enemy描画
    EnemyManager::Instance().Render(elapsedTime);

    // terrain描画
    //for (std::unique_ptr<GrassBlock>& block : grassBlock)
    //{
    //    // DrawCollを少なくするためにplayerから近いものだけ描画する
    //    float playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
    //    float blockPosX  = block.get()->model->GetTransform()->GetPosition().x;
    //
    //    float vecX = playerPosX - blockPosX;
    //    vecX = (vecX > 0) ? vecX : -vecX;
    //
    //    if (vecX < 40.0f) block->Render(elapsedTime);
    //}
    TerrainManager::Instance().Render(elapsedTime);

    // UI
    UIManager::Instance().Render(elapsedTime);
}

// debug用
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    PlayerManager::Instance().DrawDebug();

    // enemy
    EnemyManager::Instance().DrawDebug();

    // terrain
    TerrainManager::Instance().DrawDebug();

    // ui
    UIManager::Instance().DrawDebug();

#endif
}

void StagePlains::PlayerHpUiUpdate(float elapsedTime)
{
    int health = PlayerManager::Instance().GetPlayer()->GetHealth();
    
    if (UIManager::Instance().GetUiCenter())
    {
        SetUiState();
        UIManager::Instance().SetUiCenter(false);
    }
    UpdateUi(3, 1.0f, uiState, elapsedTime);
    

    switch (health)
    {
    case 0:
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetIsRender(false);
        break;
    case 1:
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetIsRender(false);
        break;
    case 2:
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetIsRender(false);
        break;
    }
}

void StagePlains::UpdateUi(int uiCount, float speed,int state,float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // ビューポート
    NO_CONST D3D11_VIEWPORT viewport = {};
    NO_CONST UINT			numViewports = 1;
    graphics.GetDeviceContext()->RSGetViewports(&numViewports, &viewport);

    // 変換行列
    const DirectX::XMMATRIX View       = Camera::Instance().GetV();
    const DirectX::XMMATRIX Projection = Camera::Instance().GetP();
    const DirectX::XMMATRIX World      = DirectX::XMMatrixIdentity();

    switch (state)
    {
    case 0:
        SetUiTimer(0.6f);
        uiState = 1;
        break;
    case 1:
        for (int i = 0; i < uiCount; ++i)
        {
            DirectX::XMFLOAT3 pos  = UIManager::Instance().GetUI(i)->GetPosition();
            DirectX::XMFLOAT2 size = UIManager::Instance().GetUI(i)->GetSize();



            // スクリーン座標変換
            NO_CONST DirectX::XMFLOAT3 screenPlPos = {};
            {
                // プレイヤーのワールド座標を取得
                NO_CONST DirectX::XMFLOAT3 worldPlPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
                worldPlPos.y += 3.0f; // プレイヤーの頭上を座標に設定

                // ワールド座標をスクリーン座標に変換
                const DirectX::XMVECTOR WorldPlPos = DirectX::XMLoadFloat3(&worldPlPos);
                const DirectX::XMVECTOR ScreenPlPos = {
                    DirectX::XMVector3Project(
                        WorldPlPos,
                        viewport.TopLeftX, viewport.TopLeftY,
                        viewport.Width,    viewport.Height,
                        viewport.MinDepth, viewport.MaxDepth,
                        Projection, View, World
                    )
                };

                // XMFLOAT型に変換
                DirectX::XMStoreFloat3(&screenPlPos, ScreenPlPos);
            }

            // スプライトの中心座標を左上から中央に調整
            const float sizeHalfX = size.x * 0.5f;
            const float sizeHalfY = size.y * 0.5f;
            displayUiPosition = { (screenPlPos.x - sizeHalfX), (screenPlPos.y - sizeHalfY),0 };


            //pos = { 450.0f, 260.0f };
            //UIManager::Instance().GetUI(i)->SetPosition(pos);

            UIManager::Instance().GetUI(i)->SetPosition(displayUiPosition);
        }
        subtractUiTimer(elapsedTime);
        if (GetUiTimer() <= 0.0f)uiState = 2;

        break;
    case 2:
        for (int i = 0; i < uiCount; ++i)
        {
            DirectX::XMFLOAT3 pos = UIManager::Instance().GetUI(i)->GetPosition();
            DirectX::XMFLOAT2 size = UIManager::Instance().GetUI(i)->GetSize();

            pos.x -= speed * 1.76f;
            pos.y -= speed;
            size.x -= speed * 2.15f;
            size.y -= speed;

            if (pos.x <= 10.0f)pos.x = 10.0f;
            if (pos.y <= 10.0f)pos.y = 10.0f;
            if (size.x <= 334.0f)size.x = 334.0f;
            if (size.y <= 160.0f)size.y = 160.0f;

            UIManager::Instance().GetUI(i)->SetPosition(pos);
            UIManager::Instance().GetUI(i)->SetSize(size);

            if ((pos.x <= 10.0f) && (pos.y <= 10.0f) && (size.x <= 334.0f) && (size.y <= 160.0f))
            {
                uiState = -1;
            }
        }
        break;
    }
}
