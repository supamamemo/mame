#include "SceneManager.h"

#include "../Graphics/Graphics.h"

#include "../Input/Input.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"

namespace Mame::Scene
{
    // ������
    void SceneManager::Initialize()
    {        
        sprite[0] = std::make_unique<Pose>(L"./resources/pose/pose.png");
        sprite[1] = std::make_unique<Pose>(L"./resources/pose/returnWhite.png");
        sprite[2] = std::make_unique<Pose>(L"./resources/pose/return.png");
        sprite[3] = std::make_unique<Pose>(L"./resources/pose/arrowWhite.png");
        sprite[4] = std::make_unique<Pose>(L"./resources/pose/arrow.png");

        sprite[1]->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));
        sprite[3]->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));
    }

    // �X�V����
    void SceneManager::Update(float elapesdTime)
    {
        if (nextScene)
        {
            // �Â��V�[�����I��
            Clear();

            // �V�����V�[����ݒ�
            currentScene = nextScene;
            nextScene = nullptr;

            // �V�[������������(�}���`�X���b�h���������Ă��Ȃ��ꍇ�ɍs��)
            if (!currentScene->IsReady())
            {
                currentScene->Initialize();
            }
        }

        if (currentScene)
        {
            if (!GetPose())
            {
                currentScene->Begin();
                currentScene->Update(elapesdTime);
                currentScene->End();
            }
            else
            {
                if (currentScene->GetSceneType() == static_cast<int>(Mame::Scene::TYPE::GAME))
                    PoseUpdateGame(elapesdTime);
            }

#ifdef USE_IMGUI
            currentScene->DrawDebug();
            DrawDebug();
#endif

            // pose�؂�ւ�����
            GamePad& gamePad = Input::Instance().GetGamePad();
            if (gamePad.GetButtonDown() & (GamePad::BTN_START | GamePad::BTN_BACK))
            {
                // sceneLoading����Ȃ������珈������
                if(currentScene->GetSceneType()!=static_cast<int>(Mame::Scene::TYPE::LOAD))
                    Mame::Scene::SceneManager::Instance().SetPose(Mame::Scene::SceneManager::Instance().GetPose() ? false : true);
            }
        }
    }

    // �`�揈��
    void SceneManager::Render(float elapsedTime)
    {
        if (!currentScene)return;

        currentScene->Render(elapsedTime);    

        if (GetPose())PoseRenderGame(elapsedTime);
    }

    void SceneManager::DrawDebug()
    {
        ImGui::Begin("sceneManager");

        ImGui::SliderInt("rasterize", &RS, 0, 3);
        ImGui::SliderInt("depthstencil", &DS, 0, 3);
        ImGui::SliderInt("samplerstate", &SS, 0, 2);

        ImGui::DragInt("state", &state);

        ImGui::End();
    }

    // �V�[���N���A
    void SceneManager::Clear()
    {
        if (!currentScene)return;
        
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }

    // pose����update(sceneTitle)
    void SceneManager::PoseUpdateTitle(float elapsedTime)
    {
    }

    // pose����update(sceneTitle)
    void SceneManager::PoseRenderTitle(float elapsedTime)
    {
    }

    // pose����update(sceneGame)
    void SceneManager::PoseUpdateGame(float elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        switch (state)
        {
        case 0:
            // �\���̐ݒ�
            sprite[1]->SetIndication(true);
            sprite[3]->SetIndication(false);
            
            // �X�e�B�b�N���ɌX������
            {
                float ax = gamePad.GetAxisLX();
                if (ax < 0)state = 1;
            }

            break;
        case 1:
            // �\���̐ݒ�
            sprite[1]->SetIndication(false);
            sprite[3]->SetIndication(true);

            // �X�e�B�b�N�E�ɌX������
            {
                float ax = gamePad.GetAxisLX();
                if (ax > 0)state = 0;
            }

            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                state = 0;
                Mame::Scene::SceneManager::Instance().SetPose(false);
                ChangeScene(new SceneTitle);
            }

            break;
        }
    }

    // pose����render(sceneGame)
    void SceneManager::PoseRenderGame(float elapsedTime)
    {
        Graphics& graphics = Graphics::Instance();
        Shader* shader = graphics.GetShader();
        shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);
        
        for (int i = 0; i < 5; ++i)
        {
            if (sprite[i]->IsIndication())sprite[i]->Render();
        }
    }


    // �V�[���؂�ւ�
    void SceneManager::ChangeScene(BaseScene* scene)
    {
        // �V�����V�[����ݒ�
        nextScene = scene;
    }
}