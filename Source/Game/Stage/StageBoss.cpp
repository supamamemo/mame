#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"

#include "../EnemyManager.h"

#include "../Terrain/TerrainBoss.h"
#include "../Terrain/TerrainManager.h"

// �R���X�g���N�^
StageBoss::StageBoss()
{
    Graphics& graphics = Graphics::Instance();

    // �X�e�[�W����&�o�^
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


    // player����
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // boss����
    EnemyManager::Instance().Register(new Boss());

    // boss��hp�p
    chefHat = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/chefHat.png");

    // �w�i��
    back = std::make_unique<Boss>("./resources/back.fbx");

    // tofu
    //EnemyTofu* tofu = new EnemyTofu();
    //tofu->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 1.5f, 10.0f));
    //tofu->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(90), 0.0f, 0.0f));
    //EnemyManager::Instance().Register(tofu);
    
}

// ������
void StageBoss::Initialize()
{
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(ToRadian(10), 0.0f, 0.0f, 0.0f));


    // �w�i��
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 2.0f, 32.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 17.0f, 8.0f));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(270), 0.0f, 0.0f));

    // �X�e�[�W�����ݒ�
    {
        TerrainManager& terrainManager = TerrainManager::Instance();

        // �ʒu�ݒ�
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

    // player������
    PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 1.0f, 10.0f));
    PlayerManager::Instance().Initialize();

    // boss������   
    EnemyManager::Instance().Initialize();
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
}

// Update�̑O�ɌĂ΂��
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

// �X�V����
void StageBoss::Update(const float& elapsedTime)
{
    Camera::Instance().UpdateBoss(elapsedTime);

    // terrain�X�V
    TerrainManager::Instance().Update(elapsedTime);

    // player�X�V
    PlayerManager& playerManager = PlayerManager::Instance();
    playerManager.Update(elapsedTime);
    
    // enemy�X�V
    EnemyManager::Instance().Update(elapsedTime);
}

// Update�̌�ɌĂ΂�鏈��
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

// �`�揈��
void StageBoss::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // terrain
    TerrainManager::Instance().Render(elapsedTime);

    // player�`��
    PlayerManager::Instance().Render(elapsedTime);

    // boss�`��
    EnemyManager::Instance().Render(elapsedTime);

    // �w�i��
    back->Render(elapsedTime);

    // tofu

    //tofu->Render(elapsedTime);

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


    ImGui::Begin("spr");
    ImGui::DragFloat2("pos", &spr.pos.x);
    ImGui::DragFloat2("texPos", &spr.texPos.x);
    ImGui::End();

#endif
}
