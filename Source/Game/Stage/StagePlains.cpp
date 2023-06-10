#include "StagePlains.h"

// �R���X�g���N�^
StagePlains::StagePlains()
{
    // player����
    player = std::make_unique<Player>();
}

// ������
void StagePlains::Initialize()
{
    // player������
    player->Initialize();
}

// �I����
void StagePlains::Finalize()
{
    // player�I����
    player->Finalize();
}

// Update�̑O�ɌĂ΂�鏈��
void StagePlains::Begin()
{
    // player
    player->Begin();
}

// �X�V����
void StagePlains::Update()
{
    // player�X�V
    player->Update();
}

// Update�̌�ɌĂ΂�鏈��
void StagePlains::End()
{
    // player
    player->End();
}

// �`�揈��
void StagePlains::Render(float elapsedTime)
{
    // player
    player->Render(elapsedTime);
}

// debug�p
void StagePlains::DrawDebug()
{
#ifdef USE_IMGUI
    // player
    player->DrawDebug();


#endif
}
