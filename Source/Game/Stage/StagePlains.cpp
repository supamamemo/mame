#include "StagePlains.h"

#include "../EnemyManager.h"
#include "../EnemyCannon.h"

// �R���X�g���N�^
StagePlains::StagePlains()
{
    // player����
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // �G�l�~�[������
    //EnemyCannon* cannon = new EnemyCannon();
    //cannon->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 20.0f));
    //EnemyManager::Instance().Register(cannon);

    EnemyCannon* cannon2 = new EnemyCannon();
    cannon2->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f));
    EnemyManager::Instance().Register(cannon2);

    // ���u���b�N����
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
    grassBlock[0] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[1] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[2] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[3] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[4] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[5] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
    grassBlock[6] = std::make_unique<GrassBlock>("./resources/temporary/assets_ground.fbx");
#endif
}

// ������
void StagePlains::Initialize()
{
    // player������
    PlayerManager::Instance().GetPlayer()->Initialize();

    // ���u���b�N������
    int temp = 0;
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {        
        block->Initialize(DirectX::XMFLOAT3(0,0,0));
        temp++;
    }
}

// �I����
void StagePlains::Finalize()
{
    // player�I����
    PlayerManager::Instance().GetPlayer()->Finalize();

    // �G�l�~�[�I����
    EnemyManager::Instance().Clear();

    // ���u���b�N�I����
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Finalize();
    }
}

// Update�̑O�ɌĂ΂�鏈��
void StagePlains::Begin()
{
    // player
    PlayerManager::Instance().GetPlayer()->Begin();

    // ���u���b�N
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Begin();
    }


    grassBlock[0].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 10.0f });
    grassBlock[1].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 12.0f });
    grassBlock[2].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 14.0f });
    grassBlock[3].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 8.0f });
    grassBlock[4].get()->model->GetTransform()->SetPosition({ 7.0f, 0.0f, 6.0f });

}

// �X�V����
void StagePlains::Update(const float& elapsedTime)
{
    // player�X�V
    PlayerManager::Instance().GetPlayer()->Update(elapsedTime);

    // �G�l�~�[�X�V
    EnemyManager::Instance().Update(elapsedTime);

    // ���u���b�N�X�V
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Update();
    }
}

// Update�̌�ɌĂ΂�鏈��
void StagePlains::End()
{
    // player
    PlayerManager::Instance().GetPlayer()->End();

    // ���u���b�N
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->End();
    }
}

// �`�揈��
void StagePlains::Render(const float& elapsedTime)
{
    // player
    PlayerManager::Instance().GetPlayer()->Render(elapsedTime);

    // �G�l�~�[�`��
    EnemyManager::Instance().Render(elapsedTime);

    // ���u���b�N
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Render(elapsedTime);
    }
}

// debug�p
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    PlayerManager::Instance().GetPlayer()->DrawDebug();

    // �G�l�~�[
    EnemyManager::Instance().DrawDebug();

    // ���u���b�N
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->DrawDebug();
    }

#endif
}
