#include "SceneLoading.h"
#include "../Graphics/Graphics.h"
#include "SceneManager.h"

SceneLoading::SceneLoading(BaseScene* nextScene) :nextScene(nextScene) 
{
    Graphics& graphics = Graphics::Instance();

    // �V�[���̑�����ݒ�
    SetSceneType(static_cast<int>(Mame::Scene::TYPE::LOAD));

    spriteDissolve = std::make_unique<SpriteDissolve>();

    //spriteAnimation = std::make_unique<SpriteAnimation>(L"./resources/mameo_Sheet.png");
    spriteAnimation = std::make_unique<SpriteAnimation>(L"./resources/mameo_Sheet1.png");
    //spriteAnimation = std::make_unique<SpriteAnimation>(L"./resources/mameo_Sheet2.png");
}

// ������
void SceneLoading::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // spriteDissolve
    spriteDissolve->Initialize();
    //spriteDissolve.Initialize(static_cast<int>(Dissolve::Fade), L"./resources/load.png");
    //spriteDissolve.Initialize(static_cast<int>(Dissolve::Fade), L"./resources/fade.jpg");
    
    spriteDissolve->SetFadeOutTexture({ 0,0,0 }, { 1280,720 }, 0.4f, 2);
    //spriteDissolve->SetFadeInTexture({ 0,0 }, { 1280,720 }, 0.4f, 6);

    spriteAnimation->Initialize(DirectX::XMFLOAT2(400.0f, 300.0f),
        DirectX::XMFLOAT2(450.0f, 183.5f), DirectX::XMFLOAT2(900.0f, 367.0f));
    

    // �X���b�h�J�n
    thread = new std::thread(LoadingThread, this);
}

// �I����
void SceneLoading::Finalize()
{
    // �X���b�h�I����
    thread->join();
    if (thread != nullptr)
    {
        delete thread;
        thread = nullptr;
    }
}

// Update�̑O�ɌĂ΂��֐�
void SceneLoading::Begin()
{
}

// �X�V����
void SceneLoading::Update(const float& elapsedTime)
{
    spriteDissolve->Update();

    spriteAnimation->PlayAnimation(20, 30, true,elapsedTime);

    //spriteDissolve.fadeOut(elapsedTime);
    //spriteDissolve->FadeIn(elapsedTime);

    if (nextScene->IsReady())
        Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);

    //// ���̃V�[���̏���������������V�[����؂�ւ���
    //if (nextScene->IsReady() &&
    //    spriteDissolve->FadeInReady(0.0f))
    //    //spriteDissolve.fadeOutReady(1.5f))
    //{
    //    Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);
    //    return;
    //}
}

// Update�̌�ɌĂ΂��
void SceneLoading::End()
{
}

// �`�揈��
void SceneLoading::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();
    ID3D11RenderTargetView* renderTargetView = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depthStencileView = graphics.GetDepthStencilView();

    // ��ʃN���A&�����_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,1.0f,0.0f,1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, color);
    deviceContext->ClearDepthStencilView(depthStencileView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencileView);

    // fade out
    {
        spriteDissolve->Render();

        spriteAnimation->Render();
    }
}

void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
    spriteDissolve->DrawDebug();

    spriteAnimation->DrawDebug();

#endif// USE_IMGUI
}

// ���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    // COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    std::ignore = CoInitialize(nullptr); // std::ignore�ŕԂ�l�x������

    // ���̃V�[���̏��������s��
    scene->nextScene->Initialize();

    // �X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    // ���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}