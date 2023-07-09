#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "../Mame.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../AudioManager.h"

#include "../../misc.h"
#include "../../texture.h"

#include "../../Game/UIManager.h"

// todo: ��ŏ���
#define FADE 1
#define MAME 1

// �R���X�g���N�^
SceneTitle::SceneTitle()
{
}

// ������
void SceneTitle::Initialize()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // ����
    {
        // �V�[���̑�����ݒ�
        SetSceneType(static_cast<int>(Mame::Scene::TYPE::TITLE));

        spriteDissolve = std::make_unique<SpriteDissolve>();

        // mameo
        //spriteLoadMameo = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/mameo_Sheet1.png");
        spriteLoadMameo = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/mameo_Sheet2.png");

        // titlePlayer����
        titlePlayer_ = std::make_unique<TitlePlayer>();

        // titleEnemyTofu����
        for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
        {
            titleEnemyTofu = std::make_unique<TitleEnemyTofu>();
        }

        // title model
        {
#if MAME
            Graphics& graphics = Graphics::Instance();
            castleModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/castel.fbx", true);
            groundModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/ground.fbx", true);
#endif
        }

        // titleUI
        {
            UIManager& uiManager = UIManager::Instance();

            uiManager.Register(new UI(L"./resources/mameoTitle.png"));  // MAMEO
            uiManager.Register(new UI(L"./resources/title.png"));       // "�{�^���������Ă�"
        }
    }


 
    // �J�����ʒu���Z�b�g
    shader->Initialize();
    
    Camera& camera = Camera::Instance();
    camera.SetAngle(0.0f);

    // fadeout�p
#if FADE
    spriteDissolve->Initialize();
    spriteDissolve->SetFadeInTexture({ 0,0,0}, { 1280,720 }, 0.4f, 2);
#else
    spriteDissolve->Initialize();
    spriteDissolve->SetFadeOutTexture({ 0,0 }, { 1280,720 }, 0.4f, 2);
#endif

    // title model
    {
#if MAME
        castleModel->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 5.45f, 0.0f));
        groundModel->GetTransform()->SetPosition(DirectX::XMFLOAT3(4.0f, -1.0f, -36.0f));
        groundModel->GetTransform()->SetScale(DirectX::XMFLOAT3(0.5f, 1.0f, 0.5f));
        groundModel->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(90), 0, 0));
#endif
    }

    const float characterScale = 3.0f;

    // titlePlayer������
    titlePlayer_->SetRotationY(ToRadian(0.0f)); // �����ʒu��ݒ�
    titlePlayer_->GetTransform()->SetScale(DirectX::XMFLOAT3(characterScale, characterScale, characterScale));
    titlePlayer_->Initialize();

    // titleEnemyTofu������
    const float rotationY = ToRadian(20.0f); // �����ʒu(�������m�̊Ԋu�ɂ��e��)
    for (int i = 0; i < ENEMY_TOFU_COUNT; ++i)
    {
        titleEnemyTofu_[i]->SetRotationY(rotationY * (i + 1)); // �����ʒu��ݒ�
        titleEnemyTofu_[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(characterScale, characterScale, characterScale));
        titleEnemyTofu_[i]->Initialize();
    }

    // UI
    {
        UIManager::Instance().GetUI(TITLESPRITE::MAMEO)->SetIsRender(true);
        UIManager::Instance().GetUI(TITLESPRITE::WORD)->SetIsRender(true);

        UIManager::Instance().GetUI(TITLESPRITE::MAMEO)->SetPosition(DirectX::XMFLOAT3(65, -115, 0));
        UIManager::Instance().GetUI(TITLESPRITE::MAMEO)->SetSize(DirectX::XMFLOAT2(1140, 490));

        UIManager::Instance().GetUI(TITLESPRITE::WORD)->SetPosition(DirectX::XMFLOAT3(420, 560, 0));
        UIManager::Instance().GetUI(TITLESPRITE::WORD)->SetSize(DirectX::XMFLOAT2(450, 170));

        UIManager::Instance().Initialize();
    }

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::Title, true); // �^�C�g��BGM�Đ�
}

// �I����
void SceneTitle::Finalize()
{
    for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
    {
        titleEnemyTofu->Finalize();
    }

    titlePlayer_->Finalize();

    // uimanager
    UIManager::Instance().Finalize();
    UIManager::Instance().Clear();

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopAllAudio(); // �S���y��~
}

// ���t���[����ԍŏ��ɌĂ΂��
void SceneTitle::Begin()
{
}

// �X�V����
void SceneTitle::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    AudioManager& audioManager = AudioManager::Instance();

    // camera
#if MAME
    Camera& camera = Camera::Instance();
    camera.UpdateTitle(elapsedTime);
#endif

    spriteDissolve->Update();

    // �{�^������������fadeOut�n�܂�
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {      
        spriteDissolve->SetFade(true);

        audioManager.StopSE(SE::Select);
        audioManager.PlaySE(SE::Select, false, true); // �I��SE�Đ�
    }

    // todo:��ŏ���
#ifdef _DEBUG
    if (gamePad.GetButtonDown() & GamePad::BTN_X)camera.SetDebugCamera();
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)camera.SetDebugCamera1();
#endif // _DEBUG


    // fadeOut
    if (spriteDissolve->IsFade())
    {
#if FADE
        spriteDissolve->FadeIn(elapsedTime);

        // fadeIn�Ō�܂łł�����
        if (spriteDissolve->FadeInReady(0.0f))
        {
            // �V�[����؂�ւ���
            Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

            // fadeIn��������Z�b�g
            spriteDissolve->SetFade(false);
        }
#else
        spriteDissolve->FadeOut(elapsedTime);

        // fadeOut�Ō�܂łł�����
        if (spriteDissolve->FadeOutReady(2.0f))
        {
            // �V�[����؂�ւ���
            Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

            // fadeOut��������Z�b�g
            spriteDissolve->SetFade(false);
        }
#endif
    }


    // UI
    UIManager::Instance().Update(elapsedTime);

    // UVScroll
    {
        if (gamePad.GetButton() & GamePad::BTN_X)
            scroll_direction.x += 0.001f;
        if (gamePad.GetButton() & GamePad::BTN_Y)
            scroll_direction.x -= 0.001f;

        // �Ȃ���UVScroll�����ł��Ă���Ȃ����炱��ő���ɂ���Ă�
        scroll_direction.x += scroll_value.x;
        scroll_direction.y += scroll_value.y;

        // float�l�̌��E���}���Ȃ��悤�ɐݒ�
        if (scroll_direction.x > 10.0f)scroll_direction.x -= 10.0f;
        if (scroll_direction.x < -10.0f)scroll_direction.x += 10.0f;
        if (scroll_direction.y > 10.0f)scroll_direction.y -= 10.0f;
        if (scroll_direction.y < -10.0f)scroll_direction.y += 10.0f;
    }

    // titlePlayer�X�V
    titlePlayer_->Update(elapsedTime);

    // titleEnemyTofu�X�V
    for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
    {
        titleEnemyTofu->Update(elapsedTime);
    }
}

// ���t���[����ԍŌ�ɌĂ΂��
void SceneTitle::End()
{
}

// �`�揈��
void SceneTitle::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
    ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

    FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
    immediate_context->ClearRenderTargetView(render_target_view, color);
    immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

    RenderContext rc;
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

    Shader* shader = graphics.GetShader();
    shader->Begin(graphics.GetDeviceContext(), rc, 0);

    // titlePlayer�`��
    titlePlayer_->Render(elapsedTime);

    // titleEnemyTofu�`��
    for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
    {
        titleEnemyTofu->Render(elapsedTime);
    }


    // title model
    {
#if MAME
        DirectX::XMMATRIX WORLD = castleModel->GetTransform()->CalcWorldMatrix(0.01f);
        DirectX::XMFLOAT4X4 world = {};
        DirectX::XMStoreFloat4x4(&world, WORLD);

        castleModel->skinned_meshes->render(graphics.GetDeviceContext(), world, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
        WORLD = groundModel->GetTransform()->CalcWorldMatrix(0.01f);
        DirectX::XMStoreFloat4x4(&world, WORLD);
        groundModel->skinned_meshes->render(graphics.GetDeviceContext(), world, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
#endif
    }

    // UI
    UIManager::Instance().Render(elapsedTime);

    // fadeOut
    {
        spriteDissolve->Render();
    }

    // mameo
    if (spriteDissolve->FadeInReady(0.3f))
    {
        spriteLoadMameo->render(graphics.GetDeviceContext(), 400.0f, 300.0f,
            450.0f, 183.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0, 0, 900.0f, 367.0f);
    }
}

// �f�o�b�O�p
void SceneTitle::DrawDebug()
{
#ifdef USE_IMGUI
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    shader->DrawDebug();

    titlePlayer_->DrawDebug();

    for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
    {
        titleEnemyTofu->DrawDebug();
    }

    // ui
    UIManager::Instance().DrawDebug();


    if (ImGui::TreeNode("scroll"))
    {
        ImGui::DragFloat2("pos", &spr[0].pos.x);
        ImGui::DragFloat2("posD", &spr[0].posD.x);

        ImGui::SliderFloat2("scroll_direction", &scroll_direction.x, -10.0f, 10.0f);
        ImGui::SliderFloat2("scroll_value", &scroll_value.x, -0.001f, 0.001f);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("dissolve"))
    {
        ImGui::DragFloat2("pos", &spr[1].pos.x);
        ImGui::DragFloat2("posD", &spr[1].posD.x);


        //dissolve_value = dissolve_value1 - 0.4f;


        ImGui::TreePop();
    }

    titlePlayer_->DrawDebug();

    spriteDissolve->DrawDebug();

    //sprite_dissolve.DrawDebug(L"./resources/ground.png");
    //sprite_dissolve->DrawDebug();

    // title model
    {
#if MAME
        ImGui::Begin("castelModel");
        castleModel->GetTransform()->DrawDebug();
        ImGui::End();
        ImGui::Begin("groundModel");
        groundModel->GetTransform()->DrawDebug();
        ImGui::End();
#endif
    }

#endif
}
