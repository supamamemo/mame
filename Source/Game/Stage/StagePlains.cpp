#include "StagePlains.h"

// �R���X�g���N�^
StagePlains::StagePlains()
{
    // player����
    player = std::make_unique<Player>();

    // ���u���b�N����
    grassBlock = std::make_unique<GrassBlock>();
}

// ������
void StagePlains::Initialize()
{
    // player������
    player->Initialize();

    // ���u���b�N������
    grassBlock->Initialize();
}

// �I����
void StagePlains::Finalize()
{
    // player�I����
    player->Finalize();

    // ���u���b�N�I����
    grassBlock->Finalize();
}

// Update�̑O�ɌĂ΂�鏈��
void StagePlains::Begin()
{
    // player
    player->Begin();

    // ���u���b�N
    grassBlock->Begin();
}

// �X�V����
void StagePlains::Update()
{
    // player�X�V
    player->Update();

    // ���u���b�N�X�V
    grassBlock->Update();
}

// Update�̌�ɌĂ΂�鏈��
void StagePlains::End()
{
    // player
    player->End();

    // ���u���b�N
    grassBlock->End();
}

// �`�揈��
void StagePlains::Render(float elapsedTime)
{
    // player
    player->Render(elapsedTime);

    // ���u���b�N
    grassBlock->Render(elapsedTime);
}

// debug�p
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    player->DrawDebug();

    // ���u���b�N
    grassBlock->DrawDebug();

#endif
}
