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
}

// ������
void StageBoss::Initialize()
{
    // player������
    PlayerManager::Instance().GetPlayer()->Initialize();

    // boss������
    boss->Initialize();

    stage[0]->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(10, 0, 10));
    stage[1]->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(4, 20, 10));
    stage[1]->model->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, DirectX::XMConvertToRadians(90), 0));
    stage[2]->model->GetTransform()->SetPosition(DirectX::XMFLOAT3(-5.5f, 20, 10));
    stage[2]->model->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, DirectX::XMConvertToRadians(90), 0));
    
    //stage[1]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
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
<<<<<<< HEAD
    //if (Collision::IntersectAABBVsAABB(&player->aabb, &boss->aabb,  resultPos))
    //{
    //    DirectX::XMFLOAT3 pos = player->model->GetTransform()->GetPosition();
    //    pos.x += resultPos.x;
    //    pos.y += resultPos.y;
    //    player->model->GetTransform()->SetPosition(pos);
    //}
=======
    PlayerManager& playerManager = PlayerManager::Instance();
    if (Collision::IntersectAABBVsAABB(&playerManager.GetPlayer()->aabb, &boss->aabb,  resultPos))
    {
        DirectX::XMFLOAT3 pos = playerManager.GetPlayer()->model->GetTransform()->GetPosition();
        pos.x += resultPos.x;
        pos.y += resultPos.y;
        playerManager.GetPlayer()->model->GetTransform()->SetPosition(pos);
    }
>>>>>>> origin/Taki

    // player�X�V
    playerManager.GetPlayer()->Update(elapsedTime);

    // boss�X�V
    boss->Update(elapsedTime);
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


#endif
}
