#include "SceneManager.h"

namespace Mame::Scene
{
    // 更新処理
    void SceneManager::Update(float elapesdTime)
    {
        if (nextScene)
        {
            // 古いシーンを終了
            Clear();

            // 新しいシーンを設定
            currentScene = nextScene;
            nextScene = nullptr;

            // シーン初期化処理(マルチスレッド処理をしていない場合に行う)
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

    // 描画処理
    void SceneManager::Render(float elapsedTime)
    {
        if (!currentScene)return;

        currentScene->Render(elapsedTime);        
    }

    // シーンクリア
    void SceneManager::Clear()
    {
        if (!currentScene)return;
        
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }


    // シーン切り替え
    void SceneManager::ChangeScene(BaseScene* scene)
    {
        // 新しいシーンを設定
        nextScene = scene;
    }
}