#include "SceneManager.h"

namespace Mame::Scene
{
    // 更新処理
    void SceneManager::Update()
    {
        if (nextScene != nullptr)
        {
            // 古いシーンを終了
            Clear();

            // 新しいシーンを設定
            currentScene = nextScene;
            nextScene = nullptr;

            // シーン初期化処理
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

    // 描画処理
    void SceneManager::Render(float elapsedTime)
    {
        if (currentScene != nullptr)
        {
            currentScene->Render(elapsedTime);
        }
    }

    // シーンクリア
    void SceneManager::Clear()
    {
        if (currentScene != nullptr)
        {
            currentScene->Finalize();
            delete currentScene;
            currentScene = nullptr;
        }
    }


    // シーン切り替え
    void SceneManager::ChangeScene(BaseScene* scene)
    {
        // 新しいシーンを設定
        nextScene = scene;
    }
}