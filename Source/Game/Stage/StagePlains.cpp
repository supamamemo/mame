#include "StagePlains.h"

#include "../../Mame/Graphics/Graphics.h"
#include "../../Mame/Graphics/Camera.h"
#include "../../Mame/Scene/SceneManager.h"
#include "../../Mame/AudioManager.h"

#include "../Terrain/TerrainNormal.h"
#include "../Terrain/TerrainFall.h"
#include "../Terrain/TerrainNoClimb.h"
#include "../Terrain/TerrainNoCollision.h"

#include "../EnemyTofu.h"
#include "../EnemyRedTofu.h"

#include "../UIManager.h"

#include "StageManager.h"
#include "StageLoading.h"
#include "StageSelection.h"

// コンストラクタ
StagePlains::StagePlains()
{
}

// 初期化
void StagePlains::Initialize()
{
    StageManager& stageManager = StageManager::Instance();

    // 生成
    {
        // terrain生成
        Terrain::nameNum = 0;
        TerrainManager& terrainManager = TerrainManager::Instance();
        RegisterTerrains(terrainManager);

        // player生成
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

        // enemy生成
        EnemyTofu::nameNum = 0;
        EnemyManager& enemyManager = EnemyManager::Instance();
        RegisterEnemies(enemyManager);


        // 背景
        Box::nameNum = 0;
        //back = std::make_unique<Box>("./resources/back.fbx");
        back = std::make_unique<Box>("./resources/tutorialBack.fbx");

        // 中間地点
        halfwayPoint_ = std::make_unique<Box>("./resources/stage/goal.fbx");

        // ゴール
        goal_ = std::make_unique<Box>("./resources/stage/goal.fbx");


        // UI
        {
            UIManager& uiManager = UIManager::Instance();
            UI::nameNum = 0;
            uiManager.Register(new UI(L"./resources/ui/baseMameHp.png"));
            uiManager.Register(new UI(L"./resources/ui/mameLeft.png"));
            uiManager.Register(new UI(L"./resources/ui/mameCenter.png"));
            uiManager.Register(new UI(L"./resources/ui/mameRight.png"));
        }
    }


    // camera初期化
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 8.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0, 0, 0));

    // terrain初期化
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        SetTerrains(terrainManager);
        terrainManager.Initialize();
    }

    // player初期化
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        // 保存してる中間地点がここのステージの中間地点だったら中間地点の位置をプレイヤーに代入
        if (stageManager.savedHalfPoint_.savedHalfwayPointStage == StageManager::SavedHalfwayPointStage::StagePlains)
        {
            playerManager.GetPlayer()->GetTransform()->SetPosition(stageManager.savedHalfPoint_.position);
            playerManager.GetPlayer()->GetTransform()->SetPositionZ(10.0f);
        }
        // 保存してる中間地点がここのステージの中間地点でなければプレイヤーに初期位置を代入し、保存した中間地点情報をリセット
        else
        {
            playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 1.5f, 10.0f));
            stageManager.savedHalfPoint_ = {}; // 保存した中間地点リセット
        }
        playerManager.Initialize();
    }

    // enemy初期化  
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        SetEnemies(enemyManager);
        enemyManager.Initialize();
    }

    // 背景
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 5, 12));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 12, 10));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0, 0));

    // 中間地点
    halfwayPoint_->GetTransform()->SetPosition(DirectX::XMFLOAT3(160.5f, 2.5f, 10.5f));
    halfwayPoint_->GetTransform()->SetScale(DirectX::XMFLOAT3(0.35f, 0.35f, 0.35f));

    // 保存してる中間地点がここのステージの中間地点だったら中間地点を回転済みにして到着フラグをtrueにする
    if (stageManager.savedHalfPoint_.savedHalfwayPointStage == StageManager::SavedHalfwayPointStage::StagePlains)
    {
        halfwayPoint_->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 0));
        isArrivedHalfwayPoint_ = true;
    }
    // 保存してる中間地点がここのステージの中間地点でなければ旗の初期回転値を代入
    else
    {
        halfwayPoint_->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(-180.0f), 0, 0));
    }


    // ゴール
    goal_->GetTransform()->SetPosition(DirectX::XMFLOAT3(277.0f, 5.5f, 10.7f));

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

        UIManager::Instance().Initialize();
    }
    
    // BGM再生
    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::StagePlains,      true);  // 草原ステージBGM再生
    audioManager.PlayBGM(BGM::StagePlains_Back, true);  // 草原ステージの環境音BGM再生
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

    // 全音楽停止
    AudioManager::Instance().StopAllAudio();
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
    }
}

// 更新処理
void StagePlains::Update(const float& elapsedTime)
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        EnemyManager&  enemyManager  = EnemyManager::Instance();
        StageManager&  stageManager  = StageManager::Instance();


        //背景
        back->BackUpdate(elapsedTime);

        // camera更新
        Camera::Instance().Update(elapsedTime);

        // terrain更新
        TerrainManager::Instance().Update(elapsedTime);

        // player更新
        playerManager.Update(elapsedTime);

        // enemy更新
        enemyManager.Update(elapsedTime);


        Transform* playerTransform  = playerManager.GetPlayer()->GetTransform();

        // 中間地点処理
        const DirectX::XMFLOAT3 halfwayPointPos = halfwayPoint_->GetTransform()->GetPosition();
        {
            // 中間地点を超えたらその中間地点情報を保存
            if (!isArrivedHalfwayPoint_)
            {
                if (playerTransform->GetPosition().x >= halfwayPointPos.x)
                {
                    stageManager.savedHalfPoint_.savedHalfwayPointStage = StageManager::SavedHalfwayPointStage::StagePlains;
                    stageManager.savedHalfPoint_.position = halfwayPointPos;
                    isArrivedHalfwayPoint_ = true;
                }
            }
            // 旗を回転させる
            else
            {
                NO_CONST float halfwayPointRotationY = halfwayPoint_->GetTransform()->GetRotation().y;
                if (halfwayPointRotationY < 0.0f)
                {
                    const float AddRotationY = ToRadian(540.) * elapsedTime;
                    halfwayPointRotationY = (std::min)(0.0f, halfwayPointRotationY + AddRotationY);
                }
                halfwayPoint_->GetTransform()->SetRotationY(halfwayPointRotationY);
            }
        }

        // プレイヤーがゴールを超えたらステージクリアにする
        const float goalPositionX = goal_->GetTransform()->GetPosition().x;
        {
            if (!isStageClear_)
            {
                if (playerTransform->GetPosition().x >= goalPositionX)
                {
                    enemyManager.AllKill(); // 敵を全員倒す
                    isStageClear_ = true;
                }
            }
            else
            {
                // プレイヤーのステートをクリアステートへ遷移
                const Player::State playerState = playerManager.GetPlayer()->GetState();
                if (playerState != Player::State::Clear) playerManager.GetPlayer()->TransitionClearState();

                // 位置を修正
                if (playerManager.GetPlayer()->GetClearState() != ClearState::MoveToRight)
                {
                    NO_CONST float playerPositionX = playerTransform->GetPosition().x;

                    const float targetPositionX = goalPositionX + 3.0f;
                    const float addPositionX = 10.0f * elapsedTime;
                    if (playerPositionX > targetPositionX)
                    {
                        playerPositionX = (std::max)(targetPositionX, (playerPositionX - addPositionX));
                    }
                    else if (playerPositionX < targetPositionX)
                    {
                        playerPositionX = (std::min)(targetPositionX, (playerPositionX + addPositionX));
                    }

                    playerTransform->SetPositionX(playerPositionX);
                }
                // 右に走り去っていったらステージセレクトに切り替える
                else
                {
                    const float moveLimitX = goalPositionX + 10.0f;
                    if (playerTransform->GetPosition().x > moveLimitX)
                    {
                        playerTransform->SetPositionX(moveLimitX);

                        stageManager.savedHalfPoint_ = {}; // 保存した中間地点リセット

                        StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
                    }
                }
            }
        }

        // 死亡処理
        if (playerTransform->GetPosition().z <= -50.0f ||
            playerTransform->GetPosition().y <= -50.0f)
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
        }

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
    }
}

// 描画処理
void StagePlains::Render(const float& elapsedTime)
{
    Graphics& graphics  = Graphics::Instance();
    Shader*   shader    = graphics.GetShader();

    // terrain描画
    TerrainManager::Instance().Render(elapsedTime);

    // 背景
    back->Render(elapsedTime);

    // 中間地点
    halfwayPoint_->Render(elapsedTime);

    // ゴール
    goal_->Render(elapsedTime);

    // player描画
    PlayerManager::Instance().Render(elapsedTime);

    // enemy描画
    EnemyManager::Instance().Render(elapsedTime);

    // UI
    UIManager::Instance().Render(elapsedTime);
}

// debug用
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // terrain
    TerrainManager::Instance().DrawDebug();

    // player
    PlayerManager::Instance().DrawDebug();

    // enemy
    EnemyManager::Instance().DrawDebug();

    // ui
    UIManager::Instance().DrawDebug();

    // 背景
    back->DrawDebug();

    // 中間地点
    halfwayPoint_->DrawDebug();

    // ゴール
    goal_->DrawDebug();

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


// 地形生成
void StagePlains::RegisterTerrains(TerrainManager& terrainManager)
{
    
    terrainManager.Register(new TerrainNormal("./resources/stage/1.fbx"));  // 0
    terrainManager.Register(new TerrainNormal("./resources/stage/2.fbx"));  // 1
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 2
    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 3
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 4

    terrainManager.Register(new TerrainNormal("./resources/stage/6.fbx"));  // 5
    terrainManager.Register(new TerrainNormal("./resources/stage/6.fbx"));  // 6
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 7
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 8
    terrainManager.Register(new TerrainNormal("./resources/stage/2.fbx"));  // 9

    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 10
    terrainManager.Register(new TerrainNormal("./resources/stage/2.fbx"));  // 11
    terrainManager.Register(new TerrainNormal("./resources/stage/2.fbx"));  // 12
    terrainManager.Register(new TerrainNormal("./resources/stage/7.fbx"));  // 13
    terrainManager.Register(new TerrainNormal("./resources/stage/7.fbx"));  // 14

    terrainManager.Register(new TerrainNormal("./resources/stage/3.fbx"));  // 15      
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 16
    terrainManager.Register(new TerrainNormal("./resources/stage/7.fbx"));  // 17
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 18
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 19

    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 20
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 21
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 22
    terrainManager.Register(new TerrainNormal("./resources/stage/1.fbx"));  // 23
    terrainManager.Register(new TerrainNoClimb("./resources/stage/5.fbx")); // 24

    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 25
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 26
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 27
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 28
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 29
                                                                            
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 30
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 31
    terrainManager.Register(new TerrainNormal("./resources/stage/6.fbx"));  // 32
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 33
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 34
                                                                            
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 35
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 36
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 37
    terrainManager.Register(new TerrainNormal("./resources/stage/8.fbx"));  // 38
    terrainManager.Register(new TerrainNormal("./resources/stage/4.fbx"));  // 39

    terrainManager.Register(new TerrainFall("./resources/stage/8.fbx"));    // 40
    terrainManager.Register(new TerrainNoClimb("./resources/stage/4.fbx")); // 41
    terrainManager.Register(new TerrainNormal("./resources/stage/1.fbx"));  // 42

    // 立て看板(当たり判定なし)
    {
        terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_yazi.fbx"));  // 43
        terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_yazi.fbx"));  // 44
        terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_drop.fbx"));  // 45
        terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_yazi.fbx"));  // 46
        terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_drop.fbx"));  // 47
        terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_yazi.fbx"));  // 48
        terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_yazi.fbx"));  // 49
        terrainManager.Register(new TerrainNoCollision("./resources/stage/flag_yazi.fbx"));  // 50
    }

    // 手前と奥の飾り地形
    {
        terrainManager.Register(new TerrainNoCollision("./resources/stage/4.fbx"));  // 51
        terrainManager.Register(new TerrainNoCollision("./resources/stage/4.fbx"));  // 52
        terrainManager.Register(new TerrainNoCollision("./resources/stage/6.fbx"));  // 53
        terrainManager.Register(new TerrainNoCollision("./resources/stage/6.fbx"));  // 54
    }
}

// エネミー生成
void StagePlains::RegisterEnemies(EnemyManager& enemyManager)
{
    enemyManager.Register(new EnemyTofu());                 // 0
    enemyManager.Register(new EnemyTofu());                 // 1
    enemyManager.Register(new EnemyTofu());                 // 2
    enemyManager.Register(new EnemyTofu());                 // 3
    enemyManager.Register(new EnemyTofu());                 // 4
    enemyManager.Register(new EnemyTofu());                 // 5
    enemyManager.Register(new EnemyTofu());                 // 6
    enemyManager.Register(new EnemyTofu());                 // 7
    enemyManager.Register(new EnemyTofu());                 // 8
    enemyManager.Register(new EnemyTofu());                 // 9
    enemyManager.Register(new EnemyTofu());                 // 10
    enemyManager.Register(new EnemyTofu());                 // 11
    enemyManager.Register(new EnemyTofu());                 // 12
    enemyManager.Register(new EnemyTofu());                 // 13
    enemyManager.Register(new EnemyTofu());                 // 14
    enemyManager.Register(new EnemyTofu());                 // 15
    enemyManager.Register(new EnemyTofu());                 // 16
    enemyManager.Register(new EnemyTofu());                 // 17
    enemyManager.Register(new EnemyTofu());                 // 18
    enemyManager.Register(new RED_TOFU::EnemyRedTofu());    // 19
    enemyManager.Register(new EnemyTofu());                 // 20
    enemyManager.Register(new EnemyTofu());                 // 21
    enemyManager.Register(new EnemyTofu());                 // 22
    enemyManager.Register(new RED_TOFU::EnemyRedTofu());    // 23
    enemyManager.Register(new RED_TOFU::EnemyRedTofu());    // 24
    enemyManager.Register(new RED_TOFU::EnemyRedTofu());    // 25
}

// 地形設定
void StagePlains::SetTerrains(TerrainManager& terrainManager)
{
    terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f,     0.0f, 10.0f));
    terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(29.0f,    0.0f, 10.0f));
    terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(18.0f,   -1.2f, 9.6f));
    terrainManager.GetTerrain(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(25.725f,  0.0f, 9.6f));
    terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(33.744f,  0.0f, 9.6f));

    terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(41.3f, 1.5f, 9.6f));
    terrainManager.GetTerrain(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(54.0f, 1.5f, 10.0f));
    terrainManager.GetTerrain(7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(62.0f, 6.0f, 10.0f));
    terrainManager.GetTerrain(8)->GetTransform()->SetPosition(DirectX::XMFLOAT3(67.0f, 4.0f, 10.0f));
    terrainManager.GetTerrain(9)->GetTransform()->SetPosition(DirectX::XMFLOAT3(78.0f, 0.0f, 10.0f));

    terrainManager.GetTerrain(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3(90.75f,   -1.0f, 9.6f));
    terrainManager.GetTerrain(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(102.0f,    0.0f, 10.0f));
    terrainManager.GetTerrain(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(124.7f,    0.0f, 10.0f));
    terrainManager.GetTerrain(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(101.5f,    7.0f, 10.0f));
    terrainManager.GetTerrain(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(113.455f,  8.0f, 10.0f));

    terrainManager.GetTerrain(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(85.55f,   0.0f, 9.6f));
    terrainManager.GetTerrain(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(113.35f, -3.0f, 10.0f));
    terrainManager.GetTerrain(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(125.0f,   7.0f, 10.0f));
    terrainManager.GetTerrain(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(136.0f,  -1.0f, 10.0f));
    terrainManager.GetTerrain(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(143.0f,   1.0f, 10.0f));

    terrainManager.GetTerrain(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(150.0f,   3.0f,  10.0f));
    terrainManager.GetTerrain(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(155.25f, -6.0f,  10.0f));
    terrainManager.GetTerrain(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(160.6f,  -5.0f,  10.0f));
    terrainManager.GetTerrain(23)->GetTransform()->SetPosition(DirectX::XMFLOAT3(186.0f,  -0.25,  10.0f));
    terrainManager.GetTerrain(24)->GetTransform()->SetPosition(DirectX::XMFLOAT3(170.35f,  10.5f, 10.0f));

    terrainManager.GetTerrain(25)->GetTransform()->SetPosition(DirectX::XMFLOAT3(176.7f, 6.525f, 10.5f));
    terrainManager.GetTerrain(26)->GetTransform()->SetPosition(DirectX::XMFLOAT3(180.5f, 7.5f,   10.5f));
    terrainManager.GetTerrain(27)->GetTransform()->SetPosition(DirectX::XMFLOAT3(184.3f, 8.4f,   10.5f));
    terrainManager.GetTerrain(28)->GetTransform()->SetPosition(DirectX::XMFLOAT3(188.1f, 7.5f,   10.5f));
    terrainManager.GetTerrain(29)->GetTransform()->SetPosition(DirectX::XMFLOAT3(191.9f, 6.525f, 10.5f));

    terrainManager.GetTerrain(30)->GetTransform()->SetPosition(DirectX::XMFLOAT3(208.4f, -2.4f,  10.0f));
    terrainManager.GetTerrain(31)->GetTransform()->SetPosition(DirectX::XMFLOAT3(215.5f,  1.0f,  10.0f));
    terrainManager.GetTerrain(32)->GetTransform()->SetPosition(DirectX::XMFLOAT3(225.0f, -2.5f,  10.0f));
    terrainManager.GetTerrain(33)->GetTransform()->SetPosition(DirectX::XMFLOAT3(225.0f,  6.75f, 10.0f));
    terrainManager.GetTerrain(34)->GetTransform()->SetPosition(DirectX::XMFLOAT3(234.5f,  1.0f,  10.0f));

    terrainManager.GetTerrain(35)->GetTransform()->SetPosition(DirectX::XMFLOAT3(242.0f,   -3.0f, 10.0f));
    terrainManager.GetTerrain(36)->GetTransform()->SetPosition(DirectX::XMFLOAT3(247.08f,  -6.5f, 10.0f));
    terrainManager.GetTerrain(37)->GetTransform()->SetPosition(DirectX::XMFLOAT3(252.167f, -3.0f, 10.0f));
    terrainManager.GetTerrain(38)->GetTransform()->SetPosition(DirectX::XMFLOAT3(247.08f,   5.5f, 10.0f));
    terrainManager.GetTerrain(39)->GetTransform()->SetPosition(DirectX::XMFLOAT3(257.25f,   0.0f, 10.0f));

    terrainManager.GetTerrain(40)->GetTransform()->SetPosition(DirectX::XMFLOAT3(261.93f,   5.706f, 10.4f));
    terrainManager.GetTerrain(41)->GetTransform()->SetPosition(DirectX::XMFLOAT3(266.2f,    11.4f,  10.0f));
    terrainManager.GetTerrain(42)->GetTransform()->SetPosition(DirectX::XMFLOAT3(279.650f, -1.2f,   10.0f));

    // 反対向かせる
    terrainManager.GetTerrain(4)->GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
    terrainManager.GetTerrain(10)->GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
    terrainManager.GetTerrain(16)->GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
    terrainManager.GetTerrain(18)->GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
    terrainManager.GetTerrain(20)->GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
    terrainManager.GetTerrain(22)->GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
    terrainManager.GetTerrain(35)->GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
    terrainManager.GetTerrain(37)->GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
    terrainManager.GetTerrain(39)->GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));

    // 立て看板
    {
        terrainManager.GetTerrain(43)->GetTransform()->SetPosition(DirectX::XMFLOAT3(2.75f,    3.5f, 10.6f));
        terrainManager.GetTerrain(44)->GetTransform()->SetPosition(DirectX::XMFLOAT3(41.25f,   8.9f, 10.6f));
        terrainManager.GetTerrain(45)->GetTransform()->SetPosition(DirectX::XMFLOAT3(57.0f,    8.9f, 10.6f));
        terrainManager.GetTerrain(46)->GetTransform()->SetPosition(DirectX::XMFLOAT3(73.0f,    3.5f, 10.6f));
        terrainManager.GetTerrain(47)->GetTransform()->SetPosition(DirectX::XMFLOAT3(151.2f,  10.0f, 10.6f));
        terrainManager.GetTerrain(48)->GetTransform()->SetPosition(DirectX::XMFLOAT3(168.0f,   3.0f, 10.6f));
        terrainManager.GetTerrain(49)->GetTransform()->SetPosition(DirectX::XMFLOAT3(208.5f,   4.5f, 10.6f));
        terrainManager.GetTerrain(50)->GetTransform()->SetPosition(DirectX::XMFLOAT3(257.25f,  7.0f, 10.6f));

        // スケールを2倍にする
        const DirectX::XMFLOAT3 scale = { 2,2,2 };
        terrainManager.GetTerrain(43)->GetTransform()->SetScale(scale);
        terrainManager.GetTerrain(44)->GetTransform()->SetScale(scale);
        terrainManager.GetTerrain(45)->GetTransform()->SetScale(scale);
        terrainManager.GetTerrain(46)->GetTransform()->SetScale(scale);
        terrainManager.GetTerrain(47)->GetTransform()->SetScale(scale);
        terrainManager.GetTerrain(48)->GetTransform()->SetScale(scale);
        terrainManager.GetTerrain(49)->GetTransform()->SetScale(scale);
        terrainManager.GetTerrain(50)->GetTransform()->SetScale(scale);

        // 裏返す
        terrainManager.GetTerrain(43)->GetTransform()->SetRotationY(ToRadian(180.0f));
        terrainManager.GetTerrain(44)->GetTransform()->SetRotationY(ToRadian(180.0f));
        terrainManager.GetTerrain(45)->GetTransform()->SetRotationY(ToRadian(180.0f));
        terrainManager.GetTerrain(46)->GetTransform()->SetRotationY(ToRadian(180.0f));
        terrainManager.GetTerrain(47)->GetTransform()->SetRotationY(ToRadian(180.0f));
        terrainManager.GetTerrain(48)->GetTransform()->SetRotationY(ToRadian(180.0f));
        terrainManager.GetTerrain(49)->GetTransform()->SetRotationY(ToRadian(180.0f));
        terrainManager.GetTerrain(50)->GetTransform()->SetRotationY(ToRadian(180.0f));
        terrainManager.GetTerrain(50)->GetTransform()->SetRotationZ(ToRadian(25.0f));   // 最後の看板は横に少し傾ける
    }

    // 手前と奥の飾り地形
    terrainManager.GetTerrain(51)->GetTransform()->SetPosition(DirectX::XMFLOAT3(38.0f,  -1.0f, -1.0f));
    terrainManager.GetTerrain(52)->GetTransform()->SetPosition(DirectX::XMFLOAT3(74.0f,   0.0f,  19.0f));
    terrainManager.GetTerrain(53)->GetTransform()->SetPosition(DirectX::XMFLOAT3(79.0f,   3.0f,  24.0f));
    terrainManager.GetTerrain(54)->GetTransform()->SetPosition(DirectX::XMFLOAT3(201.0f, -1.0f,  20.5f));

}

// エネミー設定
void StagePlains::SetEnemies(EnemyManager& enemyManager)
{
    enemyManager.GetEnemy(0 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(10.0f,  2.5f,  10.0f));
    enemyManager.GetEnemy(1 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(30.0f,  2.5f,  10.0f));
    enemyManager.GetEnemy(2 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(62.0f,  7.0f,  10.0f));
    enemyManager.GetEnemy(3 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(67.0f,  5.0f,  10.0f));
    enemyManager.GetEnemy(4 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(85.0f,  5.0f,  10.0f));

    enemyManager.GetEnemy(5 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(97.0f,  3.0f,  10.0f));
    enemyManager.GetEnemy(6 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(102.0f, 3.0f,  10.0f));
    enemyManager.GetEnemy(7 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(107.0f, 3.0f,  10.0f));
    enemyManager.GetEnemy(8 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(112.0f, 4.0f,  10.0f));
    enemyManager.GetEnemy(9 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(113.5f, 4.0f,  10.0f));

    enemyManager.GetEnemy(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3(115.0f, 4.0f, 10.0f));
    enemyManager.GetEnemy(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(120.0f, 3.0f, 10.0f));
    enemyManager.GetEnemy(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(125.0f, 3.0f, 10.0f));
    enemyManager.GetEnemy(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(129.5f, 3.0f, 10.0f));
    enemyManager.GetEnemy(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(153.3f, 1.0f, 10.0f));

    enemyManager.GetEnemy(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(154.3f, 1.0f, 10.0f));
    enemyManager.GetEnemy(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(155.3f, 1.0f, 10.0f));
    enemyManager.GetEnemy(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(156.3f, 1.0f, 10.0f));
    enemyManager.GetEnemy(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(157.3f, 1.0f, 10.0f));
    enemyManager.GetEnemy(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(176.6f, 8.0f, 10.0f));

    enemyManager.GetEnemy(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(188.0f, 8.0f, 10.0f));
    enemyManager.GetEnemy(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(191.0f, 7.0f, 10.0f));
    enemyManager.GetEnemy(22)->GetTransform()->SetPosition(DirectX::XMFLOAT3(193.0f, 7.0f, 10.0f));
    enemyManager.GetEnemy(23)->GetTransform()->SetPosition(DirectX::XMFLOAT3(225.0f, 7.5f, 10.0f));
    enemyManager.GetEnemy(24)->GetTransform()->SetPosition(DirectX::XMFLOAT3(246.0f, 0.0f, 10.0f));
    
    enemyManager.GetEnemy(25)->GetTransform()->SetPosition(DirectX::XMFLOAT3(248.0f, 0.0f, 10.0f));
}
