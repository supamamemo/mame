#include "SceneLoading.h"
#include "../Graphics/Graphics.h"
#include "SceneManager.h"

SceneLoading::SceneLoading(BaseScene* nextScene) :nextScene(nextScene) 
{
    Graphics& graphics = Graphics::Instance();

    // シーンの属性を設定
    SetSceneType(static_cast<int>(Mame::Scene::TYPE::LOAD));

    spriteDissolve = std::make_unique<SpriteDissolve>();

    spriteLoadMameo = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/mameo_Sheet.png");
}

// 初期化
void SceneLoading::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // spriteDissolve
    spriteDissolve->Initialize();
    //spriteDissolve.Initialize(static_cast<int>(Dissolve::Fade), L"./resources/load.png");
    //spriteDissolve.Initialize(static_cast<int>(Dissolve::Fade), L"./resources/fade.jpg");
    
    spriteDissolve->SetFadeOutTexture({ 0,0,0 }, { 1280,720 }, 0.4f, 2);
    //spriteDissolve->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, 6);

    
    anime.position = { 800.0f,555.0f };
    anime.size = { 450.0f,183.5f };
    anime.texSize = { 900.0f,367.0f };
    anime.animationFrame = 0;
    anime.animationTime = 0;

    // スレッド開始
    thread = new std::thread(LoadingThread, this);
}

// 終了化
void SceneLoading::Finalize()
{
    // スレッド終了化
    thread->join();
    if (thread != nullptr)
    {
        delete thread;
        thread = nullptr;
    }
}

// Updateの前に呼ばれる関数
void SceneLoading::Begin()
{
}

// 更新処理
void SceneLoading::Update(const float& elapsedTime)
{
    spriteDissolve->Update();

    anime.PlayAnimation(10, 30);

    //spriteDissolve.fadeOut(elapsedTime);
    //spriteDissolve->FadeIn(elapsedTime);

    if (nextScene->IsReady())
        Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);

    //// 次のシーンの準備が完了したらシーンを切り替える
    //if (nextScene->IsReady() &&
    //    spriteDissolve->FadeInReady(0.0f))
    //    //spriteDissolve.fadeOutReady(1.5f))
    //{
    //    Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);
    //    return;
    //}
}

// Updateの後に呼ばれる
void SceneLoading::End()
{
}

// 描画処理
void SceneLoading::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();
    ID3D11RenderTargetView* renderTargetView = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depthStencileView = graphics.GetDepthStencilView();

    // 画面クリア&レンダーターゲット設定
    FLOAT color[] = { 0.0f,1.0f,0.0f,1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, color);
    deviceContext->ClearDepthStencilView(depthStencileView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencileView);

    // fade out
    {
        spriteDissolve->Render();

        spriteLoadMameo->render(graphics.GetDeviceContext(), anime.position.x, anime.position.y,
            anime.size.x, anime.size.y, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            anime.texPos.x, anime.texPos.y, anime.texSize.x, anime.texSize.y);
    }
}

void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
    spriteDissolve->DrawDebug();

    ImGui::Begin("animation");

    ImGui::DragFloat2("pos", &anime.position.x);
    ImGui::DragFloat2("size", &anime.size.x);

    ImGui::End();

#endif// USE_IMGUI
}

// ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    std::ignore = CoInitialize(nullptr); // std::ignoreで返り値警告解消

    // 次のシーンの初期化を行う
    scene->nextScene->Initialize();

    // スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    scene->nextScene->SetReady();
}

void SceneLoading::Animation::PlayAnimation(const int& frameTime,const int& totalAnimationFrame)
{
    animationFrame = animationTime / frameTime;

    if (animationFrame >= totalAnimationFrame)
    {
        animationFrame = 0;
        animationTime = 0;
    }

    texPos.y = texSize.y * animationFrame;

    ++animationTime;
}