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
#include "../../Game/Stage/StageSelection.h"

#include "../../Game/Terrain/Terrain.h"

#include "../../Game/Stage/StageLoading.h"

// �R���X�g���N�^
SceneGame::SceneGame()
{
    Graphics& graphics = Graphics::Instance();

    // �V�[���̑�����ݒ�
    SetSceneType(static_cast<int>(Mame::Scene::TYPE::GAME));

    spriteDissolve = std::make_unique<SpriteDissolve>();
    

    // mameo
    spriteLoadMameo = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/mameo_Sheet.png");

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
        spriteDissolve->SetFadeInTexture({ 0,0 ,0}, { 1280,720 }, 0.4f, 2);
        break;
    case 1:
        spriteDissolve->Initialize();
        spriteDissolve->SetFadeOutTexture({ 0,0,0 }, { 1280,720 }, 0.4f, 2);
        break;
    }
    
    spriteDissolve->SetFade(true);



    // �J�������Œ�l�ɂ��Ă���
    // ����܂��ǂ������
    Camera& camera = Camera::Instance();
    camera.GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 2, 0));

    //StageManager::Instance().ChangeStage(new StagePlains);
    //StageManager::Instance().ChangeStage(new StageBoss);
    //StageManager::Instance().ChangeStage(new StageTutorial);
    StageManager::Instance().ChangeStage(new StageSelection);
}

// �I����
void SceneGame::Finalize()
{
    StageManager::Instance().Clear();
}

// Update�̑O�ɌĂяo�����
void SceneGame::Begin()
{
    //StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
}

// �X�V����
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

    // �ʃX���b�h���Ƀf�o�C�X�R���e�L�X�g���g��ꂽ�ꍇ��
    // �����A�N�Z�X���Ȃ��悤�ɔr�����䂷��
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
        spriteDissolve->Render();
    }

    // mameo
    if (!spriteDissolve->FadeOutReady(0.5f))
    {
        spriteLoadMameo->render(graphics.GetDeviceContext(), 400.0f, 300.0f,
            450.0f, 183.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0, 0, 900.0f, 367.0f);
    }
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
        Terrain::nameNum = 0;   // �ԍ����Z�b�g
        StageManager::Instance().ChangeStage(new StageLoading(new StageTutorial));
    }
    if (ImGui::Button("plains"))
    {
        Terrain::nameNum = 0;   // �ԍ����Z�b�g
        StageManager::Instance().ChangeStage(new StageLoading(new StagePlains));
    }
    if (ImGui::Button("boss"))
    {
        Terrain::nameNum = 0;   // �ԍ����Z�b�g
        StageManager::Instance().ChangeStage(new StageLoading(new StageBoss));

    }
    if (ImGui::Button("select"))
    {
        Terrain::nameNum = 0;   // �ԍ����Z�b�g
        StageManager::Instance().ChangeStage(new StageLoading(new StageSelection));
    }

    ImGui::Begin("renderLengthXLimit_");
    ImGui::SliderFloat("renderLengthXLimit_", &Terrain::renderLengthXLimit_, 0.0f, 50.0f);
    ImGui::End();
}