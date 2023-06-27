#include "SceneGame.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneTitle.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "../../Game/Stage/StageManager.h"
#include "../../Game/Stage/StagePlains.h"
#include "../../Game/Stage/StageBoss.h"


// コンストラクタ
SceneGame::SceneGame()
{
    spriteDissolve[0] = std::make_unique<SpriteDissolve>();
    spriteDissolve[1] = std::make_unique<SpriteDissolve>(L"./resources/fade.jpg");
}

// 初期化
void SceneGame::Initialize()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    
    // カメラの位置リセット
    shader->Initialize();

    // fadeout用
    switch (fadeType)
    {
    case 0:
        spriteDissolve[0]->Initialize();
        spriteDissolve[0]->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, 6);
        break;
    case 1:
        spriteDissolve[0]->Initialize();
        spriteDissolve[0]->SetFadeOutTexture({ 0,0 }, { 1280,720 }, 0.4f, 6);
        break;
    }
    
    spriteDissolve[0]->SetFade(true);

    //spriteDissolve[1]->Initialize();
    //spriteDissolve[1]->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, 6);
    //spriteDissolve[1]->SetFade(true);



    // カメラを固定値にしている
    // これまたどっかやる
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2, 0));

    //StageManager::Instance().ChangeStage(new StagePlains);
    StageManager::Instance().ChangeStage(new StageBoss);
}

// 終了化
void SceneGame::Finalize()
{
    StageManager::Instance().Clear();
}

// Updateの前に呼び出される
void SceneGame::Begin()
{
    if (changeStage)
    {
        StageManager::Instance().ChangeStage(new StageBoss);
        changeStage = false;
    }
    if (changeStage1)
    {
        StageManager::Instance().ChangeStage(new StagePlains);
        changeStage1 = false;
    }
}

// 更新処理
void SceneGame::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    spriteDissolve[0]->Update();
    //spriteDissolve[1]->Update();
    //if (spriteDissolve[1]->IsFade())
    //{
    //    spriteDissolve[1]->FadeIn(elapsedTime);

    //    // fadeIn最後までできたら
    //    if (spriteDissolve[1]->FadeInReady(0.0f))
    //    {
    //        // fadeIn判定をリセット
    //        spriteDissolve[1]->SetFade(false);
    //    }
    //}

    if (spriteDissolve[0]->IsFade())
    {
        switch (fadeType)
        {
        case 0:
            spriteDissolve[0]->FadeIn(elapsedTime);

            // fadeIn最後までできたら
            if (spriteDissolve[0]->FadeInReady(0.0f))
            {
                // fadeIn判定をリセット
                spriteDissolve[0]->SetFade(false);
            }
            break;
        case 1:
            spriteDissolve[0]->FadeOut(elapsedTime);

            // fadeOut最後までできたら
            if (spriteDissolve[0]->FadeOutReady(2.0f))
            {
                // シーンを切り替える
                //Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

                // fadeOut判定をリセット
                spriteDissolve[0]->SetFade(false);
            }
            break;
        }
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        //Mame::Scene::SceneManager::Instance().ChangeScene(new SceneTitle);
    }

    // stage更新
    StageManager::Instance().Update(elapsedTime);

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

    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
    immediate_context->ClearRenderTargetView(render_target_view, color);
    immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

    RenderContext rc;
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

    Shader* shader = graphics.GetShader();
    shader->Begin(graphics.GetDeviceContext(), rc);


    StageManager::Instance().Render(elapsedTime);

    // fadeOut
    {
        spriteDissolve[0]->Render();
        //spriteDissolve[1]->Render();
    }
}

// debug用
void SceneGame::DrawDebug()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    shader->DrawDebug();

    spriteDissolve[0]->DrawDebug();

    ImGui::SliderInt("textureType", &textureType, 0, 11);
    if (ImGui::Button("fadeIn"))
    {
        spriteDissolve[0]->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, textureType);
        spriteDissolve[0]->SetFade(true);
        fadeType = 0;
    }
    if (ImGui::Button("fadeOut"))
    {
        spriteDissolve[0]->SetFadeOutTexture({ 0,0 }, { 1280,720 }, 0.4f, textureType);
        spriteDissolve[0]->SetFade(true);
        fadeType = 1;
    }

    //if (ImGui::Button("fade"))
    //{
    //    spriteDissolve[1]->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, textureType);
    //    spriteDissolve[1]->SetFade(true);
    //    fadeType = 0;
    //}

    if (ImGui::Button("terrain"))
        changeStage = true;
    if (ImGui::Button("stageP"))
        changeStage1 = true;

}
