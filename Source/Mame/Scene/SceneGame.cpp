#include "SceneGame.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneTitle.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "../../Game/Stage/StageManager.h"
#include "../../Game/Stage/StagePlains.h"
#include "../../Game/Stage/StageBoss.h"


// �R���X�g���N�^
SceneGame::SceneGame()
{
    spriteDissolve = std::make_unique<SpriteDissolve>();    
}

// ������
void SceneGame::Initialize()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    
    // �J�����̈ʒu���Z�b�g
    shader->Initialize();

    // fadeout�p
    switch (fadeType)
    {
    case 0:
        spriteDissolve->Initialize();
        spriteDissolve->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, 6);
        break;
    case 1:
        spriteDissolve->Initialize();
        spriteDissolve->SetFadeOutTexture({ 0,0 }, { 1280,720 }, 0.4f, 6);
        break;
    }
    
    spriteDissolve->SetFade(true);

    // �J�������Œ�l�ɂ��Ă���
    // ����܂��ǂ������
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2, 0));

    //StageManager::Instance().ChangeStage(new StagePlains);
    StageManager::Instance().ChangeStage(new StageBoss);
}

// �I����
void SceneGame::Finalize()
{
    StageManager::Instance().Clear();
}

// Update�̑O�ɌĂяo�����
void SceneGame::Begin()
{
}

// �X�V����
void SceneGame::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (spriteDissolve->IsFade())
    {
        switch (fadeType)
        {
        case 0:
            spriteDissolve->FadeIn(elapsedTime);

            // fadeIn�Ō�܂łł�����
            if (spriteDissolve->FadeInReady(0.0f))
            {
                // fadeIn��������Z�b�g
                spriteDissolve->SetFade(false);
            }
            break;
        case 1:
            spriteDissolve->FadeOut(elapsedTime);

            // fadeOut�Ō�܂łł�����
            if (spriteDissolve->FadeOutReady(2.0f))
            {
                // �V�[����؂�ւ���
                //Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

                // fadeOut��������Z�b�g
                spriteDissolve->SetFade(false);
            }
            break;
        }
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneTitle);
    }

    // stage�X�V
    StageManager::Instance().Update(elapsedTime);

}

// Update�̌�ɌĂяo�����
void SceneGame::End()
{
}

// �`�揈��
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


    // fadeOut
    {
        //spriteDissolve->Render();
    }

    StageManager::Instance().Render(elapsedTime);
}

// debug�p
void SceneGame::DrawDebug()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    shader->DrawDebug();

    spriteDissolve->DrawDebug();

    ImGui::SliderInt("textureType", &textureType, 0, 11);
    if (ImGui::Button("fadeIn"))
    {
        spriteDissolve->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, textureType);
        spriteDissolve->SetFade(true);
        fadeType = 0;
    }
    if (ImGui::Button("fadeOut"))
    {
        spriteDissolve->SetFadeOutTexture({ 0,0 }, { 1280,720 }, 0.4f, textureType);
        spriteDissolve->SetFade(true);
        fadeType = 1;
    }

}
