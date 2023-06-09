#include "SceneGame.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneTitle.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "../../Game/Stage/StageManager.h"
#include "../../Game/Stage/StagePlains.h"

void SceneGame::Initialize()
{
    player = new Player();
    player->Initialize();

    StageManager::Instance().ChangeStage(new StagePlains);
}

void SceneGame::Finalize()
{
    player->Finalize();
}

void SceneGame::Begin()
{
}

void SceneGame::Update()
{
    Begin();

    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneTitle);
    }

    // stage�X�V
    StageManager::Instance().Update();

    player->Update();

    End();
}

void SceneGame::End()
{
}

void SceneGame::Render(float elapsedTime)
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

    // �v���C���[�`��
    player->Render(elapsedTime);
}

void SceneGame::DrawDebug()
{
    player->DrawDebug();
}