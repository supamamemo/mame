#include "SceneManager.h"

namespace Mame::Scene
{
    // �X�V����
    void SceneManager::Update()
    {
        if (nextScene != nullptr)
        {
            // �Â��V�[�����I��
            Clear();

            // �V�����V�[����ݒ�
            currentScene = nextScene;
            nextScene = nullptr;

            // �V�[������������
            currentScene->Initialize();
        }

        if (currentScene != nullptr)
        {
            currentScene->Begin();
            currentScene->Update();
#ifdef USE_IMGUI
            currentScene->DrawDebug();
#endif
            currentScene->End();
        }
    }

    // �`�揈��
    void SceneManager::Render(float elapsedTime)
    {
        if (currentScene != nullptr)
        {
            currentScene->Render(elapsedTime);
        }
    }

    // �V�[���N���A
    void SceneManager::Clear()
    {
        if (currentScene != nullptr)
        {
            currentScene->Finalize();
            delete currentScene;
            currentScene = nullptr;
        }
    }


    // �V�[���؂�ւ�
    void SceneManager::ChangeScene(BaseScene* scene)
    {
        // �V�����V�[����ݒ�
        nextScene = scene;
    }
}