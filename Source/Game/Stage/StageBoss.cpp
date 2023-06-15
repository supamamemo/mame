#include "StageBoss.h"

#include "../../Mame/Graphics/Graphics.h"



// �R���X�g���N�^
StageBoss::StageBoss()
{
    // player����
    player = std::make_unique<Player>();

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
    player->Initialize();

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
    player->Finalize();

    // boss�I����
    boss->Finalize();
}

// Update�̑O�ɌĂ΂��
void StageBoss::Begin()
{
    // player
    player->Begin();

    // boss
    boss->Begin();
}

// �X�V����
void StageBoss::Update(const float& elapsedTime)
{
    // player�X�V
    player->Update(elapsedTime);

    // boss�X�V
    boss->Update();

    DirectX::XMFLOAT2 resultPos{};
    if (Collision::IntersectAABBVsAABB(&boss->aabb, &player->aabb,  resultPos))
    {
        DirectX::XMFLOAT3 pos = player->model->GetTransform()->GetPosition();
        pos.x += resultPos.x;
        pos.y += resultPos.y;
        player->model->GetTransform()->SetPosition(pos);
    }
}

// Update�̌�ɌĂ΂�鏈��
void StageBoss::End()
{
    // player
    player->End();

    // boss
    boss->End();
}

// �`�揈��
void StageBoss::Render(const float& elapsedTime)
{
    // player�`��
    player->Render(elapsedTime);

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
    player->DrawDebug();

    // boss
    boss->DrawDebug();

    for (std::unique_ptr<Boss>& temp : stage)
    {
        temp->DrawDebug();
    }


#endif
}
