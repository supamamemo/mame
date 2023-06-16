#include "StagePlains.h"

// �R���X�g���N�^
StagePlains::StagePlains()
{
    // player����
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();

    // ���u���b�N����
#if 0
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block = std::make_unique<GrassBlock>();
    }
#else
    grassBlock[0] = std::make_unique<GrassBlock>("./resources/temporary/assets_air_ground.fbx");
    grassBlock[1] = std::make_unique<GrassBlock>("./resources/temporary/assets_air_ground_move.fbx");
    grassBlock[2] = std::make_unique<GrassBlock>("./resources/temporary/assets_block.fbx");
    grassBlock[3] = std::make_unique<GrassBlock>("./resources/temporary/assets_cannon.fbx");
    grassBlock[4] = std::make_unique<GrassBlock>("./resources/temporary/assets_cannon_bullet.fbx");
    grassBlock[5] = std::make_unique<GrassBlock>("./resources/temporary/assets_flag.fbx");
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
}

// �X�V����
void StagePlains::Update(const float& elapsedTime)
{
    // player�X�V
    PlayerManager::Instance().GetPlayer()->Update(elapsedTime);

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

    // ���u���b�N
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->DrawDebug();
    }

#endif
}
