#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"

// �R���X�g���N�^
StageBoss::StageBoss()
{
    // player����
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // boss����
    boss = std::make_unique<Boss>();

    // ��
    for (std::unique_ptr<Boss>& temp : stage)
    {
        temp = std::make_unique<Boss>("./resources/temporary/assets_ground.fbx");
    }

    // �w�i��
    back = std::make_unique<Boss>("./resources/back.fbx");

    // tofu
    tofu = std::make_unique<EnemyTofu>();
}

// ������
void StageBoss::Initialize()
{
    // player������
    PlayerManager::Instance().GetPlayer()->Initialize();

    // boss������
    boss->Initialize();

    stage[0]->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 1, 10));
    stage[0]->GetTransform()->SetScale(DirectX::XMFLOAT3(4.0f, 1.0f, 1.0f));
    stage[1]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-11, 20, 10));
    stage[1]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0));
    stage[2]->GetTransform()->SetPosition(DirectX::XMFLOAT3(9.5f, 20, 10));
    stage[2]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, ToRadian(90), 0));
    
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 10.0f, -12.0f));
    camera.GetTransform()->SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(10), 0.0f, 0.0f, 0.0f));


    //tofu
    tofu->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 1.0f, 10.0f));
    tofu->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(90), 0.0f, 0.0f));

    // �w�i��
    back->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 5.0f, 20.0f));
    back->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 8.0f, 13.0f));
    back->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(-90), 0.0f, 0.0f));
}

// �I����
void StageBoss::Finalize()
{
    // player�I����
    PlayerManager::Instance().GetPlayer()->Finalize();

    // boss�I����
    boss->Finalize();
}

// Update�̑O�ɌĂ΂��
void StageBoss::Begin()
{
    // player
    PlayerManager::Instance().GetPlayer()->Begin();

    // boss
    boss->Begin();
}

// �X�V����
void StageBoss::Update(const float& elapsedTime)
{
    DirectX::XMFLOAT3 resultPos{};

    //if (Collision::IntersectAABBVsAABB(&player->aabb, &boss->aabb,  resultPos))
    //{
    //    DirectX::XMFLOAT3 pos = player->model->GetTransform()->GetPosition();
    //    pos.x += resultPos.x;
    //    pos.y += resultPos.y;
    //    player->model->GetTransform()->SetPosition(pos);
    //}

    PlayerManager& playerManager = PlayerManager::Instance();
    //if (Collision::IntersectAABBVsAABB(&playerManager.GetPlayer()->aabb, &boss->aabb,  resultPos))
    //{
    //    DirectX::XMFLOAT3 pos = playerManager.GetPlayer()->model->GetTransform()->GetPosition();
    //    pos.x += resultPos.x;
    //    pos.y += resultPos.y;
    //    playerManager.GetPlayer()->model->GetTransform()->SetPosition(pos);
    //}


    // player�X�V
    playerManager.GetPlayer()->Update(elapsedTime);

    // boss�X�V
    boss->Update(elapsedTime);

    // tofu
    tofu->Update(elapsedTime);
}

// Update�̌�ɌĂ΂�鏈��
void StageBoss::End()
{
    // player
    PlayerManager::Instance().GetPlayer()->End();

    // boss
    boss->End();
}

// �`�揈��
void StageBoss::Render(const float& elapsedTime)
{
    // player�`��
    PlayerManager::Instance().GetPlayer()->Render(elapsedTime);

    // boss�`��
    boss->Render(elapsedTime);


    // ��
    for (std::unique_ptr<Boss>& temp : stage)
    {
        temp->Render(elapsedTime);
    }

    // �w�i��
    back->Render(elapsedTime);

    // tofu
    tofu->Render(elapsedTime);
}

// debug�p
void StageBoss::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    PlayerManager::Instance().GetPlayer()->DrawDebug();

    // boss
    boss->DrawDebug();

    for (std::unique_ptr<Boss>& temp : stage)
    {
        temp->DrawDebug();
    }

    // �w�i��
    back->DrawDebug();

    tofu->DrawDebug();
#endif
}
