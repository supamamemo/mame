#include "StagePlains.h"

#include "../../Mame/Graphics/Camera.h"
#include "../EnemyManager.h"
#include "../EnemyTofu.h"
#include "../Terrain/TerrainManager.h"
#include "../Terrain/TerrainPlains.h"

// �R���X�g���N�^
StagePlains::StagePlains()
{
    // terrain����
    TerrainManager& terrainManager = TerrainManager::Instance();
    {
        terrainManager.Register(new TerrainPlains("./resources/stage/1.fbx" )); // 0
        terrainManager.Register(new TerrainPlains("./resources/stage/2.fbx" )); // 1
        terrainManager.Register(new TerrainPlains("./resources/stage/3.fbx" )); // 2
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 3
        terrainManager.Register(new TerrainPlains("./resources/stage/6.fbx" )); // 4
        terrainManager.Register(new TerrainPlains("./resources/stage/6.fbx" )); // 5
        terrainManager.Register(new TerrainPlains("./resources/stage/7.fbx" )); // 6
        terrainManager.Register(new TerrainPlains("./resources/stage/7.fbx" )); // 7
        terrainManager.Register(new TerrainPlains("./resources/stage/2.fbx" )); // 8
        terrainManager.Register(new TerrainPlains("./resources/stage/4.fbx" )); // 9
    }

    // player����
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // enemy����
    EnemyManager& enemyManager = EnemyManager::Instance();
    {
        // 23�̐���
        for (int i = 0; i < 23; ++i)
        {
            enemyManager.Register(new EnemyTofu());
        }
    }
}

// ������
void StagePlains::Initialize()
{
    // camera������
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 10, -12));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0, 0, 0));


    const float posZ = 10.0f; // ���s��

    // terrain������
    {
        TerrainManager& terrainManager = TerrainManager::Instance();
        // �����Œn�ʂ���ׂĂ��܂�
        {
            terrainManager.GetTerrain(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, posZ));
            terrainManager.GetTerrain(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(29.0f, 0.0f, posZ));
            terrainManager.GetTerrain(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(26.0f, 0.0f, posZ));
            terrainManager.GetTerrain(3)->GetTransform()->SetPosition(DirectX::XMFLOAT3(34.0f, 0.0f, posZ));
            terrainManager.GetTerrain(4)->GetTransform()->SetPosition(DirectX::XMFLOAT3(42.0f, 2.0f, posZ));
            terrainManager.GetTerrain(5)->GetTransform()->SetPosition(DirectX::XMFLOAT3(54.0f, 2.0f, posZ));
            terrainManager.GetTerrain(6)->GetTransform()->SetPosition(DirectX::XMFLOAT3(64.0f, 6.0f, posZ));
            terrainManager.GetTerrain(7)->GetTransform()->SetPosition(DirectX::XMFLOAT3(72.0f, 4.0f, posZ));
            terrainManager.GetTerrain(8)->GetTransform()->SetPosition(DirectX::XMFLOAT3(86.0f, 0.0f, posZ));
            terrainManager.GetTerrain(9)->GetTransform()->SetPosition(DirectX::XMFLOAT3(92.0f, -1.0f, posZ));
        }
        terrainManager.Initialize();
    }

    // player������
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        playerManager.GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 1.5f, posZ));
        playerManager.Initialize();
    }

    // enemy������  
    {
        EnemyManager& enemyManager = EnemyManager::Instance();

        // �ʒu�ݒ�
        {
            enemyManager.GetEnemy(0 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.0f,   1.5f,  posZ));
            enemyManager.GetEnemy(1 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.0f,   20.5f,  posZ));
            enemyManager.GetEnemy(2 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.0f,   30.5f,  posZ));
            //enemyManager.GetEnemy(3 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.5f,   40.5f,  posZ));
            //enemyManager.GetEnemy(4 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.0f,   50.5f,  posZ));
            //enemyManager.GetEnemy(5 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(11.0f,   60.5f,  posZ));
            /*enemyManager.GetEnemy(1 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(13.0f,   1.5f,  posZ));
            enemyManager.GetEnemy(2 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(15.0f,   1.5f,  posZ));
            enemyManager.GetEnemy(3 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(24.5f,   1.5f,  posZ));
            enemyManager.GetEnemy(4 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(33.0f,   1.5f,  posZ));
            enemyManager.GetEnemy(5 )->GetTransform()->SetPosition(DirectX::XMFLOAT3(40.0f,   1.5f,  posZ));*/
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
    
}

// �I����
void StagePlains::Finalize()
{
    // �G�l�~�[�I����
    EnemyManager::Instance().Finalize();
    EnemyManager::Instance().Clear();

    // player�I����
    PlayerManager::Instance().Finalize();

    // terrain�I����
    TerrainManager::Instance().Finalize();
    TerrainManager::Instance().Clear();
}

// Update�̑O�ɌĂ΂�鏈��
void StagePlains::Begin()
{
    // player
    PlayerManager::Instance().Begin();

    // enemy
    EnemyManager::Instance().Begin();

    // terrain
    TerrainManager::Instance().Begin();
}

// �X�V����
void StagePlains::Update(const float& elapsedTime)
{
    // camera�X�V
    Camera::Instance().Update(elapsedTime);

    // player�X�V
    PlayerManager::Instance().Update(elapsedTime);

    // enemy�X�V
    EnemyManager::Instance().Update(elapsedTime);

    // terrain�X�V
    TerrainManager::Instance().Update(elapsedTime);
}

// Update�̌�ɌĂ΂�鏈��
void StagePlains::End()
{
    // player
    PlayerManager::Instance().End();

    // enemy
    EnemyManager::Instance().End();

    // terrain
    TerrainManager::Instance().End();
}

// �`�揈��
void StagePlains::Render(const float& elapsedTime)
{
    // player�`��
    PlayerManager::Instance().Render(elapsedTime);

    // enemy�`��
    EnemyManager::Instance().Render(elapsedTime);

    // terrain�`��
    //for (std::unique_ptr<GrassBlock>& block : grassBlock)
    //{
    //    // DrawColl�����Ȃ����邽�߂�player����߂����̂����`�悷��
    //    float playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
    //    float blockPosX  = block.get()->model->GetTransform()->GetPosition().x;
    //
    //    float vecX = playerPosX - blockPosX;
    //    vecX = (vecX > 0) ? vecX : -vecX;
    //
    //    if (vecX < 40.0f) block->Render(elapsedTime);
    //}
    TerrainManager::Instance().Render(elapsedTime);
}

// debug�p
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    PlayerManager::Instance().DrawDebug();

    // enemy
    EnemyManager::Instance().DrawDebug();

    // terrain
    TerrainManager::Instance().DrawDebug();

#endif
}
