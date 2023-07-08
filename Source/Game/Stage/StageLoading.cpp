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

    // スレッド開始
    thread = new std::thread(LoadingThread, this);
}

void StageLoading::Finalize()
{
    // スレッド終了化
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
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    std::ignore = CoInitialize(nullptr); // std::ignoreで返り値警告解消

    // 次のシーンの初期化を行う
    stage->nextStage->Initialize();

    // スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    stage->nextStage->SetReady();
}
