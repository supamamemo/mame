#include "StagePlains.h"

// �R���X�g���N�^
StagePlains::StagePlains()
{
    // player����
    player = std::make_unique<Player>();

    // ���u���b�N����
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block = std::make_unique<GrassBlock>();
    }
}

// ������
void StagePlains::Initialize()
{
    // player������
    player->Initialize();

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
    player->Finalize();

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
    player->Begin();

    // ���u���b�N
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->Begin();
    }    
}

// �X�V����
void StagePlains::Update()
{
    // player�X�V
    player->Update();

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
    player->End();

    // ���u���b�N
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->End();
    }
}

// �`�揈��
void StagePlains::Render(float elapsedTime)
{
    // player
    player->Render(elapsedTime);

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
    player->DrawDebug();

    // ���u���b�N
    for (std::unique_ptr<GrassBlock>& block : grassBlock)
    {
        block->DrawDebug();
    }

#endif
}
