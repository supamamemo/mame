#include "StageLoading.h"
#include "StageManager.h"

#include "../../Mame/Graphics/Graphics.h"

StageLoading::StageLoading(Stage* nextStage):nextStage(nextStage)
{
}

void StageLoading::Initialize()
{
    // �X�e�[�W�̑�����ݒ�
    SetStageType(static_cast<int>(Mame::Stage::TYPE::LOAD));

    spriteDissolve = std::make_unique<SpriteDissolve>();


    spriteAnimation[0] = std::make_unique<SpriteAnimation>(L"./resources/mameo_Sheet2.png");
    spriteAnimation[1] = std::make_unique<SpriteAnimation>(L"./resources/loadingsheet.png");

    // spriteDissolve
    spriteDissolve->Initialize();

    spriteDissolve->SetFadeOutTexture({ 0,0,0 }, { 1280,720 }, 0.4f, 2);
 
    spriteAnimation[0]->Initialize(DirectX::XMFLOAT2(320.0f, 210.0f),
        DirectX::XMFLOAT2(540.0f, 220.2f), DirectX::XMFLOAT2(900.0f, 367.0f));
    spriteAnimation[1]->Initialize(DirectX::XMFLOAT2(400.0f, 440.0f),
        DirectX::XMFLOAT2(450.0f, 180.0f), DirectX::XMFLOAT2(900.0f, 367.0f));

    // �X���b�h�J�n
    thread = new std::thread(LoadingThread, this);
}

void StageLoading::Finalize()
{
    // �X���b�h�I����
    thread->join();
    if (thread != nullptr)
    {
        delete thread;
        thread = nullptr;
    }
}

void StageLoading::Begin()
{
}

void StageLoading::Update(const float& elapsedTime)
{
    spriteDissolve->Update();

    const float frameSpeed = 30.0f;   // �t���[�����x
    const int   totalFrame = 30;      // �t���[���̖���
    spriteAnimation[0]->PlayAnimation(elapsedTime, frameSpeed, totalFrame, true);
    spriteAnimation[1]->PlayAnimation(elapsedTime, frameSpeed, totalFrame, true);

    if (nextStage->IsReady())
        StageManager::Instance().ChangeStage(nextStage);
}

void StageLoading::End()
{
}

void StageLoading::Render(const float& elapsedTime)
{
    // fade out
    {
        spriteDissolve->Render();

        spriteAnimation[0]->Render();
        spriteAnimation[1]->Render();
    }
}

void StageLoading::DrawDebug()
{
#ifdef USE_IMGUI
    spriteDissolve->DrawDebug();

    spriteAnimation[0]->DrawDebug();
    spriteAnimation[1]->DrawDebug();

#endif// USE_IMGUI
}

void StageLoading::LoadingThread(StageLoading* stage)
{
    // COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    std::ignore = CoInitialize(nullptr); // std::ignore�ŕԂ�l�x������

    // ���̃V�[���̏��������s��
    stage->nextStage->Initialize();

    // �X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    // ���̃V�[���̏��������ݒ�
    stage->nextStage->SetReady();
}
