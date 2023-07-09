#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"
#include "../../Mame/Scene/SceneManager.h"
#include "../../Mame/AudioManager.h"

#include "../EnemyManager.h"

#include "../Terrain/TerrainBoss.h"
#include "../Terrain/TerrainManager.h"

#include "../UIManager.h"

#include "../BossStateDerived.h"

// �R���X�g���N�^
StageBoss::StageBoss()
{
}

// ������
void StageBoss::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // ����
    {
        // �X�e�[�W����&�o�^
        {
            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/ground.fbx"));

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx"));

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/wall.fbx"));

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/ceiling.fbx"));

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/door.fbx"));

            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/door.fbx"));


            TerrainManager::Instance().Register(new TerrainBoss("./resources/bossStage/ground.fbx"));
        }

        // player����
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

        // boss����
        EnemyManager::Instance().Register(new Boss());

        // boss��hp�p
        chefHat = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/chefHat.png");

        // �w�i��
        back = std::make_unique<Box>("./resources/back.fbx");

        // UI
        {
            // mameoHP
            UIManager::Instance().Register(new UI(L"./resources/ui/baseMameHp.png"));
            UIManager::Instance().Register(new UI(L"./resources/ui/mameLeft.png"));
            UIManager::Instance().Register(new UI(L"./resources/ui/mameCenter.png"));
            UIManager::Instance().Register(new UI(L"./resources/ui/mameRight.png"));

            // bossHP

            UIManager::Instance().Initialize();
        }
    }

    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(-20.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0.0f, 0.0f, 0.0f));
    
    camera.SetIsShake();
    camera.SetCameraMoveY();

    // �w�i��
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 6.0f, 12.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

    // �X�e�[�W�����ݒ�
    {
        TerrainManager& terrainManager = TerrainManager::Instance();

        // �ʒu�ݒ�
        {
            terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 1, 10));

            terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-10, 11, 10));

            terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 11, 10));

            terrainManager.GetTerrain(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 11, 10));
            terrainManager.GetTerrain(3)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.72f, 1.0f, 1.0f));

            terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-10, 8, 10));
            terrainManager.GetTerrain(4)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.9f, 0.9f, 0.9f));
            // terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-10, 4, 10)); // ���܂�����
           
            terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 8, 10));
            //terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 4, 10));
            terrainManager.GetTerrain(5)->GetTransform()->SetScale(DirectX::XMFLOAT3(0.9f, 0.9f, 0.9f));
            
            terrainManager.GetTerrain(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-24.0f, 1.0f, 10.0f));
        }
        
        // materialColor
        //terrainManager.GetTerrain(0)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));
        //terrainManager.GetTerrain(1)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));
        //terrainManager.GetTerrain(2)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));
        //terrainManager.GetTerrain(3)->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.64f, 0.0f, 1.0f));

        terrainManager.Initialize();
    }

    // player������
    PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(-28.0f, 1.0f, 10.0f));
    PlayerManager::Instance().Initialize();

    // boss������   
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

        UIManager::Instance().GetUI(UISPRITE::BaseMameHp)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpLeft)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpCenter)->SetIsRender(true);
        UIManager::Instance().GetUI(UISPRITE::mameHpRight)->SetIsRender(true);

        UIManager::Instance().Initialize();
    }

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::StageBoss_Back, true); // �{�X�����BGM�Đ�
}

// �I����
void StageBoss::Finalize()
{
    // boss�I����
    //boss->Finalize();
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear(); // vector�N���A

    // player�I����
    PlayerManager::Instance().Finalize();

    // terrain�I����
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear(); // vector�N���A

    // ui�I����
    UIManager::Instance().Finalize();
    UIManager::Instance().Clear();

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopAllAudio(); // �S���y��~
}

// Update�̑O�ɌĂ΂��
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

// �X�V����
void StageBoss::Update(const float& elapsedTime)
{
    if (!Mame::Scene::SceneManager::Instance().isHitStop_)
    {
        // terrain�X�V
        TerrainManager::Instance().Update(elapsedTime);

        // player�X�V
        PlayerManager& playerManager = PlayerManager::Instance();
        playerManager.Update(elapsedTime);

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
            // enemy�X�V
            EnemyManager& enemyManager = EnemyManager::Instance();
            enemyManager.Update(elapsedTime);

            // �{�X�����s�X�e�[�g�Ȃ���sSE�Đ�
            const int bossCurrentState = enemyManager.GetEnemy(0)->GetStateMachine()->GetStateIndex();
            const int bossStateWalk    = static_cast<int>(BOSS::STATE::Walk);
            if (bossCurrentState == bossStateWalk)
            {
                AudioManager& audioManager = AudioManager::Instance();
                audioManager.PlaySE(SE::Boss_Walk, true); // ���sSE�Đ�
            }
        }

        Camera::Instance().UpdateBoss(elapsedTime);
    }

    // �h�A
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
    else
    {
        DirectX::XMFLOAT3 terrainPos = TerrainManager::Instance().GetTerrain(5)->GetTransform()->GetPosition();
        terrainPos.y += elapsedTime;
        if (terrainPos.y >= 8.0f)terrainPos.y = 8.0f;
        TerrainManager::Instance().GetTerrain(5)->GetTransform()->SetPosition(terrainPos);
    }

    // playerHp
    PlayerHpUiUpdate(elapsedTime);

    // bossHp

}

// Update�̌�ɌĂ΂�鏈��
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

// �`�揈��
void StageBoss::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // �w�i��
    back->Render(elapsedTime, 1.0f);

    // terrain
    TerrainManager::Instance().Render(elapsedTime);

    // player�`��
    PlayerManager::Instance().Render(elapsedTime);

    // boss�`��
    EnemyManager::Instance().Render(elapsedTime);

    //playerHp
    UIManager::Instance().Render(elapsedTime);

    // bossHp
    shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);
    chefHat->render(graphics.GetDeviceContext(), spr.pos.x, spr.pos.y, spr.texPos.x, spr.texPos.y);

}

// debug�p
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
    

    // �w�i��
    back->DrawDebug();

    // ui
    UIManager::Instance().DrawDebug();

    ImGui::Begin("spr");
    ImGui::DragFloat2("pos", &spr.pos.x);
    ImGui::DragFloat2("texPos", &spr.texPos.x);
    ImGui::End();

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

void StageBoss::UpdateUi(int uiCount, float speed, int state, float elapsedTime)
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