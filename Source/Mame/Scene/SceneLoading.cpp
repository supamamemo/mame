#include "SceneLoading.h"
#include "../Graphics/Graphics.h"
#include "SceneManager.h"

// ������
void SceneLoading::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // spriteDissolve
    spriteDissolve.Initialize(static_cast<int>(Dissolve::Fade), L"./resources/load.png");
    spriteDissolve.SetFadeOutTexture({ 0,0 }, { 1280,720 }, 0.4f, 2);

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

void SceneLoading::Begin()
{
}

// �X�V����
void SceneLoading::Update(float elapesdTime)
{
    spriteDissolve.fadeOut(elapesdTime);

    // ���̃V�[���̏���������������V�[����؂�ւ���
    if (nextScene->IsReady() &&
        spriteDissolve.fadeOutReady(1.5f))
    {
        Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);
        return;
    }
}

void SceneLoading::End()
{
}

// �`�揈��
void SceneLoading::Render(float elapsedTime)
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
        spriteDissolve.Render();
    }
}

void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
    spriteDissolve.DrawDebug(L"./resources/load.png");
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