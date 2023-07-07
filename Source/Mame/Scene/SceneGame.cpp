#include "SceneGame.h"

#include "../Graphics/EffectManager.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SceneTitle.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "../../Game/Stage/StageManager.h"
#include "../../Game/Stage/StagePlains.h"
#include "../../Game/Stage/StageBoss.h"
#include "../../Game/Stage/StageTutorial.h"

#include "../../Game/Terrain/Terrain.h"


// �R���X�g���N�^
SceneGame::SceneGame()
{
    // �V�[���̑�����ݒ�
    SetSceneType(static_cast<int>(Mame::Scene::TYPE::GAME));

    spriteDissolve[0] = std::make_unique<SpriteDissolve>();
    spriteDissolve[1] = std::make_unique<SpriteDissolve>(L"./resources/fade.jpg");
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
        spriteDissolve[0]->Initialize();
        spriteDissolve[0]->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, 2);
        break;
    case 1:
        spriteDissolve[0]->Initialize();
        spriteDissolve[0]->SetFadeOutTexture({ 0,0 }, { 1280,720 }, 0.4f, 2);
        break;
    }
    
    spriteDissolve[0]->SetFade(true);

    //spriteDissolve[1]->Initialize();
    //spriteDissolve[1]->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, 6);
    //spriteDissolve[1]->SetFade(true);



    // �J�������Œ�l�ɂ��Ă���
    // ����܂��ǂ������
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2, 0));

    StageManager::Instance().ChangeStage(new StagePlains);
    //StageManager::Instance().ChangeStage(new StageBoss);
    //StageManager::Instance().ChangeStage(new StageTutorial);
}

// �I����
void SceneGame::Finalize()
{
    StageManager::Instance().Clear();
}

// Update�̑O�ɌĂяo�����
void SceneGame::Begin()
{
    // �X�e�[�W�̐؂�ւ�
    if (GetChangeStageTutorial())
    {
        StageManager::Instance().Clear();
        StageManager::Instance().ChangeStage(new StageTutorial);
        SetChangeStageTutorial();
    }
    if (GetChangeStagePlains())
    {
        StageManager::Instance().Clear();
        StageManager::Instance().ChangeStage(new StagePlains);
        SetChangeStagePlains();
    }
    if (GetChangeStageBoss())
    {
        StageManager::Instance().Clear();
        StageManager::Instance().ChangeStage(new StageBoss);
        SetChangeStageBoss();
    }
}

// �X�V����
void SceneGame::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    spriteDissolve[0]->Update();
    //spriteDissolve[1]->Update();
    //if (spriteDissolve[1]->IsFade())
    //{
    //    spriteDissolve[1]->FadeIn(elapsedTime);

    //    // fadeIn�Ō�܂łł�����
    //    if (spriteDissolve[1]->FadeInReady(0.0f))
    //    {
    //        // fadeIn��������Z�b�g
    //        spriteDissolve[1]->SetFade(false);
    //    }
    //}

    if (spriteDissolve[0]->IsFade())
    {
        switch (fadeType)
        {
        case 0:
            spriteDissolve[0]->FadeIn(elapsedTime);

            // fadeIn�Ō�܂łł�����
            if (spriteDissolve[0]->FadeInReady(0.0f))
            {
                // fadeIn��������Z�b�g
                spriteDissolve[0]->SetFade(false);
            }
            break;
        case 1:
            spriteDissolve[0]->FadeOut(elapsedTime);

            // fadeOut�Ō�܂łł�����
            if (spriteDissolve[0]->FadeOutReady(2.0f))
            {
                // �V�[����؂�ւ���
                //Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

                // fadeOut��������Z�b�g
                spriteDissolve[0]->SetFade(false);
            }
            break;
        }
    }



    // stage�X�V
    StageManager::Instance().Update(elapsedTime);

    // �G�t�F�N�g�X�V����
    EffectManager::Instance().Update(elapsedTime);
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


    StageManager::Instance().Render(elapsedTime);

    // 3D�G�t�F�N�g�`��
    {
        Camera& camera = Camera::Instance();
        DirectX::XMFLOAT4X4 view, projection;
        DirectX::XMStoreFloat4x4(&view, camera.GetV());
        DirectX::XMStoreFloat4x4(&projection, camera.GetP());

        EffectManager::Instance().Render(view, projection);
    }

    // fadeOut
    {
        spriteDissolve[0]->Render();
        //spriteDissolve[1]->Render();
    }
}

// debug�p
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

    if (ImGui::Button("tutorial"))
    {
        Terrain::nameNum = 0;   // �ԍ����Z�b�g
        ChangeStage(static_cast<int>(Mame::Scene::STAGE::Tutorial));
    }
    if (ImGui::Button("plains"))
    {
        Terrain::nameNum = 0;   // �ԍ����Z�b�g
        ChangeStage(static_cast<int>(Mame::Scene::STAGE::Plains));
    }
    if (ImGui::Button("boss"))
    {
        Terrain::nameNum = 0;   // �ԍ����Z�b�g
        ChangeStage(static_cast<int>(Mame::Scene::STAGE::Boss));
    }

    ImGui::Begin("renderLengthXLimit_");
    ImGui::SliderFloat("renderLengthXLimit_", &Terrain::renderLengthXLimit_, 0.0f, 50.0f);
    ImGui::End();
}
