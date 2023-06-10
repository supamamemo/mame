#include "SceneManager.h"

namespace Mame::Scene
{
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
            currentScene->Begin();
            currentScene->Update(elapesdTime);
#ifdef USE_IMGUI
            currentScene->DrawDebug();
#endif
            currentScene->End();
        }
    }

    // �`�揈��
    void SceneManager::Render(float elapsedTime)
    {
        if (!currentScene)return;

        currentScene->Render(elapsedTime);        
    }

    // �V�[���N���A
    void SceneManager::Clear()
    {
        if (!currentScene)return;
        
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }


    // �V�[���؂�ւ�
    void SceneManager::ChangeScene(BaseScene* scene)
    {
        // �V�����V�[����ݒ�
        nextScene = scene;
    }
}