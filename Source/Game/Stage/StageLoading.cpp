#include "StageLoading.h"
#include "StageManager.h"

#include "../../Mame/Graphics/Graphics.h"

StageLoading::StageLoading(Stage* nextStage):nextStage(nextStage)
{
    spriteDissolve = std::make_unique<SpriteDissolve>();

    spriteAnimation = std::make_unique<SpriteAnimation>(L"./resources/mameo_Sheet.png");
}

void StageLoading::Initialize()
{
    // spriteDissolve
    spriteDissolve->Initialize();

    spriteDissolve->SetFadeOutTexture({ 0,0,0 }, { 1280,720 }, 0.4f, 2);
 
    spriteAnimation->Initialize(DirectX::XMFLOAT2(400.0f, 300.0f),
        DirectX::XMFLOAT2(450.0f, 183.5f), DirectX::XMFLOAT2(900.0f, 367.0f));

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

    spriteAnimation->PlayAnimation(10, 30, true);

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

        spriteAnimation->Render();
    }
}

void StageLoading::DrawDebug()
{
#ifdef USE_IMGUI
    spriteDissolve->DrawDebug();

    spriteAnimation->DrawDebug();

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
