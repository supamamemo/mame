#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"
#include "../../Mame/Scene/SceneManager.h"
#include "../../Mame/AudioManager.h"

#include "../EnemyManager.h"

#include "../Terrain/TerrainBoss.h"
#include "../Terrain/TerrainManager.h"

#include "../UIManager.h"

#include "../BossStateDerived.h"

#include "StageManager.h"
#include "StageLoading.h"
#include "StageSelection.h"

// コンストラクタ
StageBoss::StageBoss()
{
}

// 初期化
void StageBoss::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    StageManager& stageManager = StageManager::Instance();

    // ステージの属性を設定
    SetStageType(static_cast<int>(Mame::Stage::TYPE::BOSS));

    // 生成
    {
        // ステージ生成&登録
        {
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/ground.fbx"));   // 0

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx"));     // 1
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx"));     // 2

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/ceiling.fbx"));  // 3

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/door.fbx"));     // 4
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/door.fbx"));     // 5

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/ground.fbx"));   // 6

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/ceiling.fbx"));  // 7 第二の床
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/ceiling.fbx"));  // 8 第三の床

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 9    壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 10   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 11   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 12   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 13   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 14   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 15   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 16   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 17   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 18   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 19   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 20   壁
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx")); // 21   壁
        }

        // player生成
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

        // boss生成
        EnemyManager::Instance().Register(new Boss());

        // 背景仮
        back = std::make_unique<Box>("./resources/back.fbx");

        // UI
        {
            // mameoHP
            UIManager::Instance().Register(new UI(L"./resources/ui/baseMameHp.png"));
            UIManager::Instance().Register(new UI(L"./resources/ui/mameLeft.png"));
            UIManager::Instance().Register(new UI(L"./resources/ui/mameCenter.png"));
            UIManager::Instance().Register(new UI(L"./resources/ui/mameRight.png"));

            // bossHP
            UIManager::Instance().Register(new UI(L"./resources/ui/baseBossHp.png"));
            UIManager::Instance().Register(new UI(L"./resources/ui/bossLeft.png"));
            UIManager::Instance().Register(new UI(L"./resources/ui/bossCenter.png"));
            UIManager::Instance().Register(new UI(L"./resources/ui/bossRight.png"));

            UIManager::Instance().Initialize();
        }
    }

    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(-20.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0.0f, 0.0f, 0.0f));
    
    camera.SetIsShake();
    camera.SetCameraMoveY();
    camera.Initialize();

    // 背景仮
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 6.0f, 12.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

    // ステージ初期設定
    {
        TerrainManager& terrainManager = TerrainManager::Instance();

        // 位置設定
        {
            terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 1, 10));     // 右の床地形
            terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-10, 11, 10));  // 室内左の壁
            terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 11, 10));   // 室内右の壁

            terrainManager.GetTerrain(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.1f, 11, 10)); // 天井
            terrainManager.GetTerrain(3)->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.5f));

            terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-10, 8, 10));   //　左の扉
            terrainManager.GetTerrain(4)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.9f, 0.9f, 0.9f));
            // terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-10, 4, 10)); // しまった後
           
            terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 8, 10));    //　右の扉
            //terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 4, 10));
            terrainManager.GetTerrain(5)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.9f, 0.9f, 0.9f));
            
            terrainManager.GetTerrain(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-24.0f, 1.0f, 10.0f)); // 左の床地形

            terrainManager.GetTerrain(7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.1f, 13.0f, 10)); // 第二の天井
            terrainManager.GetTerrain(7)->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.5f));
            terrainManager.GetTerrain(8)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.1f, 15.0f, 10)); // 第三の天井
            terrainManager.GetTerrain(8)->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 0.5f));

            terrainManager.GetTerrain(9 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(-11.0f, 7.0f, 12.0f)); // 室内左の壁の後ろの壁
            terrainManager.GetTerrain(10)->GetTransform()->SetPosition(DirectX::XMFLOAT3( 10.9f, 7.0f, 12.0f)); // 室内右の壁の後ろの壁

            terrainManager.GetTerrain(11)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-13.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(12)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-15.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(13)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-17.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(14)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-19.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(15)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-21.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(16)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-23.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(17)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-25.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(18)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-27.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(19)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-29.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(20)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-31.0f, 7.0f, 12.0f)); // 壁
            terrainManager.GetTerrain(21)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-33.0f, 7.0f, 12.0f)); // 壁

            // 色を暗めに設定
            static DirectX::XMFLOAT4 materialColor = { 0.7f, 0.7f, 0.7f, 1.0f };
            for (int i = 9; i <= 21; ++i)
            {
                terrainManager.GetTerrain(i)->SetMaterialColor(materialColor);
            }
        }
        
        // materialColor
        //terrainManager.GetTerrain(0)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));
        //terrainManager.GetTerrain(1)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));
        //terrainManager.GetTerrain(2)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));
        //terrainManager.GetTerrain(3)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));

        terrainManager.Initialize();
    }

    // player初期化
    PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-28.0f, 1.0f, 10.0f));
    PlayerManager::Instance().Initialize();
    stageManager.savedHalfPoint_ = {};

    // boss初期化   
    EnemyManager::Instance().Initialize();

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

        UIManager::Instance().GetUI(UISPRITE::BaseBossHp)->SetPosition(bossUiPos);
        UIManager::Instance().GetUI(UISPRITE::BaseBossHp)->SetSize(bossUiSize);
        UIManager::Instance().GetUI(UISPRITE::BossHpLeft)->SetPosition(bossUiPos);
        UIManager::Instance().GetUI(UISPRITE::BossHpLeft)->SetSize(bossUiSize);
        UIManager::Instance().GetUI(UISPRITE::BossHpCenter)->SetPosition(bossUiPos);
        UIManager::Instance().GetUI(UISPRITE::BossHpCenter)->SetSize(bossUiSize);
        UIManager::Instance().GetUI(UISPRITE::BossHpRight)->SetPosition(bossUiPos);
        UIManager::Instance().GetUI(UISPRITE::BossHpRight)->SetSize(bossUiSize);

        UIManager::Instance().GetUI(UISPRITE::BaseMameHp)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetIsRender(true);

        UIManager::Instance().GetUI(UISPRITE::BaseBossHp)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::BossHpLeft)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::BossHpCenter)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::BossHpRight)->SetIsRender(true);

        UIManager::Instance().Initialize();
    }

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::StageBoss_Back, true); // ボス戦環境音BGM再生
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

    // ui終了化
    UIManager::Instance().Finalize();
    UIManager::Instance().Clear();

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopAllAudio(); // 全音楽停止
}

// Updateの前に呼ばれる
void StageBoss::Begin()
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        // player
        PlayerManager::Instance().Begin();

        // boss
        EnemyManager::Instance().Begin();
    }
}

// 更新処理
void StageBoss::Update(const float& elapsedTime)
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        Camera::Instance().UpdateBoss(elapsedTime);

        // terrain更新
        TerrainManager::Instance().Update(elapsedTime);

        // player更新
        PlayerManager& playerManager = PlayerManager::Instance();
        playerManager.Update(elapsedTime);

        // enemy更新
        EnemyManager& enemyManager = EnemyManager::Instance();
        if (!isEnemyUpdate_)
        {
            if (PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x >= -11.0f
                && Camera::Instance().GetTransform()->GetPosition().x == 0.0f)
            {
                isEnemyUpdate_ = true;
            }
        }
        else
        {
            // enemy更新
            enemyManager.Update(elapsedTime);

            // ボスが歩行ステートなら歩行SE再生
            const int bossCurrentState = enemyManager.GetEnemy(0)->GetStateMachine()->GetStateIndex();
            const int bossStateWalk    = static_cast<int>(BOSS::STATE::Walk);
            if (bossCurrentState == bossStateWalk)
            {
                AudioManager& audioManager = AudioManager::Instance();
                audioManager.PlaySE(SE::Boss_Walk, true); // 歩行SE再生
            }
        }



        // ステージクリア処理
        Transform* playerTransform = playerManager.GetPlayer()->GetTransform();
        {
            if (isStageClear_)
            {
                clearTimer_ += elapsedTime;

                if (clearTimer_ >= 3.0f)
                {
                    clearTimer_ = 3.0f;

                    Camera::Instance().isParabolaMove_ = true;

                    // 右に走り去っていったらステージセレクトに切り替える
                    if (playerManager.GetPlayer()->GetClearState() == ClearState::MoveToRight)
                    {
                        const float moveLimitX = 7.5f;
                        if (playerTransform->GetPosition().x > moveLimitX)
                        {
                            playerTransform->SetPositionX(moveLimitX);

                            //stageManager.savedHalfPoint_ = {}; // 保存した中間地点リセット


                            Mame::Scene::SceneManager::Instance().selectState =
                                (std::max)(Mame::Scene::SceneManager::Instance().selectState, 3);

                            StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
                        }
                    }
                }
            }
        }

        // 死亡処理
        if (playerTransform->GetPosition().z <= -25.0f ||
            playerTransform->GetPosition().y <= -40.0f)
        {
            StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
        }
    }

    // ドア
    if (PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x > -9.0f)
    {
        DirectX::XMFLOAT3 terrainPos = TerrainManager::Instance().GetTerrain(4)->GetTransform()->GetPosition();
        terrainPos.y -= elapsedTime;
        if (terrainPos.y <= 4.0f)terrainPos.y = 4.0f;
        TerrainManager::Instance().GetTerrain(4)->GetTransform()->SetPosition(terrainPos);
    }

    if (EnemyManager::Instance().GetEnemy(0)->GetHealth() > 0)
    {
        if (EnemyManager::Instance().GetEnemy(0)->GetTransform()->GetPosition().x <= 8.5f)
        {
            DirectX::XMFLOAT3 terrainPos = TerrainManager::Instance().GetTerrain(5)->GetTransform()->GetPosition();
            terrainPos.y -= elapsedTime;
            if (terrainPos.y <= 4.0f)terrainPos.y = 4.0f;
            TerrainManager::Instance().GetTerrain(5)->GetTransform()->SetPosition(terrainPos);
        }
    }
    //else
    //{
    //    DirectX::XMFLOAT3 terrainPos = TerrainManager::Instance().GetTerrain(5)->GetTransform()->GetPosition();
    //    terrainPos.y += elapsedTime;
    //    if (terrainPos.y >= 8.0f)terrainPos.y = 8.0f;
    //    TerrainManager::Instance().GetTerrain(5)->GetTransform()->SetPosition(terrainPos);
    //}

    // playerHp
    PlayerHpUiUpdate(elapsedTime);

    // bossHp
    BossHpUiUpdate(elapsedTime);
}

// Updateの後に呼ばれる処理
void StageBoss::End()
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        // player
        PlayerManager::Instance().End();

        // boss
        //boss->End();
        EnemyManager::Instance().End();
    }
}

// 描画処理
void StageBoss::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // 背景仮
    back->Render(elapsedTime, 1.0f);

    // terrain
    TerrainManager::Instance().Render(elapsedTime);

    // player描画
    PlayerManager::Instance().Render(elapsedTime);

    // boss描画
    EnemyManager::Instance().Render(elapsedTime);

    //playerHp & bossHp
    UIManager::Instance().Render(elapsedTime);
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

    // 背景仮
    back->DrawDebug();

    // ui
    UIManager::Instance().DrawDebug();
#endif
}

void StageBoss::PlayerHpUiUpdate(float elapsedTime)
{
    int health = PlayerManager::Instance().GetPlayer()->GetHealth();

    if (UIManager::Instance().GetUiCenter())
    {
        SetUiState();
        UIManager::Instance().SetUiCenter(false);
    }
    PlayerUpdateUi(3, 1.0f, uiState, elapsedTime);


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

void StageBoss::PlayerUpdateUi(int uiCount, float speed, int state, float elapsedTime)
{
    switch (state)
    {
    case 0:
        SetUiTimer(0.6f);
        uiState = 1;

        break;
    case 1:
        for (int i = 0; i < uiCount; ++i)
        {
            displayUiPosition = { 425,260,10 };

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

            pos.x -= speed * 1.63f;
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

void StageBoss::BossHpUiUpdate(float elapsedTime)
{
    int health = EnemyManager::Instance().GetEnemy(0)->GetHealth();

    //if (UIManager::Instance().GetUiCenter())
    //{
    //    SetUiState();
    //    UIManager::Instance().SetUiCenter(false);
    //}
    //PlayerUpdateUi(3, 1.0f, uiState, elapsedTime);


    switch (health)
    {
    case 0:
        UIManager::Instance().GetUI(UISPRITE::BossHpLeft)->SetIsRender(false);
        break;
    case 1:
        UIManager::Instance().GetUI(UISPRITE::BossHpCenter)->SetIsRender(false);
        break;
    case 2:
        UIManager::Instance().GetUI(UISPRITE::BossHpRight)->SetIsRender(false);
        break;
    }
}

void StageBoss::BossUpdateUi(int uiCount, float speed, int state, float elapsedTime)
{
}
