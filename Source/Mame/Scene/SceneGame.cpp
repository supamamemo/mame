#include "SceneGame.h"

#include "../../Game/Stage/StageManager.h"
#include "../../Game/Stage/StageLoading.h"
#include "../../Game/Stage/StageTutorial.h"
#include "../../Game/Stage/StageSelection.h"
#include "../../Game/Stage/StagePlains.h"
#include "../../Game/Stage/StageBoss.h"
#include "../../Game/Stage///StageEx.h"

#include "../../Game/Terrain/Terrain.h"

#include "../Graphics/EffectManager.h"
#include "../Graphics/Graphics.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SceneTitle.h"

#include "../Input/Input.h"

#include "../AudioManager.h"

// コンストラクタ
SceneGame::SceneGame()
{
}

// 初期化
void SceneGame::Initialize()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    
    // シーンの属性を設定
    SetSceneType(static_cast<int>(Mame::Scene::TYPE::GAME));

    {
        spriteDissolve = std::make_unique<SpriteDissolve>();

        // mameo
        spriteLoadMameo = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/mameo_Sheet2.png");
    }

    // カメラの位置リセット
    shader->Initialize();

    // fadeout用
    switch (fadeType)
    {
    case 0:
        spriteDissolve->Initialize();
        spriteDissolve->SetFadeInTexture({ 0,0 ,0}, { 1280,720 }, 0.4f, 2);
        break;
    case 1:
        spriteDissolve->Initialize();
        spriteDissolve->SetFadeOutTexture({ 0,0,0 }, { 1280,720 }, 0.4f, 2);
        break;
    }
    
    spriteDissolve->SetFade(true);


    // カメラを固定値にしている
    // これまたどっかやる
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2, 0));


    StageManager::Instance().ChangeStage(new StageSelection);
    //StageManager::Instance().ChangeStage(new StageTutorial);
    //StageManager::Instance().ChangeStage(new StagePlains);
    //StageManager::Instance().ChangeStage(new StageBoss);
    //StageManager::Instance().ChangeStage(new StageEx);
}

// 終了化
void SceneGame::Finalize()
{
    StageManager::Instance().Clear();

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopAllAudio(); // 全音楽停止
}

// Updateの前に呼び出される
void SceneGame::Begin()
{
    //StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
}

// 更新処理
void SceneGame::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    spriteDissolve->Update();

    if (spriteDissolve->IsFade())
    {
        switch (fadeType)
        {
        case 0:
            spriteDissolve->FadeIn(elapsedTime);

            // fadeIn最後までできたら
            if (spriteDissolve->FadeInReady(0.0f))
            {
                // fadeIn判定をリセット
                spriteDissolve->SetFade(false);
            }
            break;
        case 1:
            spriteDissolve->FadeOut(elapsedTime);

            // fadeOut最後までできたら
            if (spriteDissolve->FadeOutReady(2.0f))
            {
                // シーンを切り替える
                //Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

                // fadeOut判定をリセット
                spriteDissolve->SetFade(false);
            }
            break;
        }
    }



    // stage更新
    StageManager::Instance().Update(elapsedTime);

    // エフェクト更新処理
    EffectManager::Instance().Update(elapsedTime);
}

// Updateの後に呼び出される
void SceneGame::End()
{
}

// 描画処理
void SceneGame::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
    ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

    // 別スレッド中にデバイスコンテキストが使われた場合に
    // 同時アクセスしないように排他制御する
    std::lock_guard<std::mutex> lock(graphics.GetMutex());

    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
    immediate_context->ClearRenderTargetView(render_target_view, color);
    immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

    RenderContext rc;
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

    Shader* shader = graphics.GetShader();
    shader->Begin(graphics.GetDeviceContext(), rc);


    StageManager::Instance().Render(elapsedTime);

    // 3Dエフェクト描画
    {
        Camera& camera = Camera::Instance();
        DirectX::XMFLOAT4X4 view, projection;
        DirectX::XMStoreFloat4x4(&view, camera.GetV());
        DirectX::XMStoreFloat4x4(&projection, camera.GetP());

        EffectManager::Instance().Render(view, projection);
    }

    // fadeOut
    {
        spriteDissolve->Render();
    }

    // mameo
    if (!spriteDissolve->FadeOutReady(0.5f))
    {
        spriteLoadMameo->render(graphics.GetDeviceContext(), 320.0f, 210.0f,
            540.0f, 220.2f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0, 0, 900.0f, 367.0f);
    }
}

// debug用
void SceneGame::DrawDebug()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    shader->DrawDebug();

    spriteDissolve->DrawDebug();

    ImGui::SliderInt("textureType", &textureType, 0, 11);
    if (ImGui::Button("fadeIn"))
    {
        spriteDissolve->SetFadeInTexture({ 0,0,0 }, { 1280,720 }, 0.4f, textureType);
        spriteDissolve->SetFade(true);
        fadeType = 0;
    }
    if (ImGui::Button("fadeOut"))
    {
        spriteDissolve->SetFadeOutTexture({ 0,0,0 }, { 1280,720 }, 0.4f, textureType);
        spriteDissolve->SetFade(true);
        fadeType = 1;
    }

    //if (ImGui::Button("fade"))
    //{
    //    spriteDissolve[1]->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, textureType);
    //    spriteDissolve[1]->SetFade(true);
    //    fadeType = 0;
    //}

    if (ImGui::Button("tutorial"))
    {
        Terrain::nameNum = 0;   // 番号リセット
        StageManager::Instance().ChangeStage(new StageLoading(new StageTutorial));
    }
    if (ImGui::Button("plains"))
    {
        Terrain::nameNum = 0;   // 番号リセット
        StageManager::Instance().ChangeStage(new StageLoading(new StagePlains));
    }
    if (ImGui::Button("boss"))
    {
        Terrain::nameNum = 0;   // 番号リセット
        StageManager::Instance().ChangeStage(new StageLoading(new StageBoss));

    }
    if (ImGui::Button("select"))
    {
        Terrain::nameNum = 0;   // 番号リセット
        StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
    }

    ImGui::Begin("renderLengthXLimit_");
    ImGui::SliderFloat("renderLengthXLimit_", &Terrain::renderLengthXLimit_, 0.0f, 50.0f);
    ImGui::End();
}