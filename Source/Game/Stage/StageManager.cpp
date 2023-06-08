#include "StageManager.h"

// 更新処理
void StageManager::Update()
{
    if (nextStage != nullptr)
    {
        // 古いstageを終了
        Clear();

        // 新しいstageを設定
        currentStage = nextStage;
        nextStage = nullptr;

        // stage初期化処理
        currentStage->Initialize();
    }

    if (currentStage != nullptr)
    {
        currentStage->Begin();
        currentStage->Update();
#ifdef USE_IMGUI
        currentStage->DrawDebug();
#endif
        currentStage->End();
    }
}

// 描画処理
void StageManager::Render(float elapsedTime)
{
    if (currentStage != nullptr)
    {
        currentStage->Render(elapsedTime);
    }
}

// stageクリア
void StageManager::Clear()
{
    if (currentStage != nullptr)
    {
        currentStage->Finalize();
        delete currentStage;
        currentStage = nullptr;
    }
}

// stage切り替え
void StageManager::ChangeStage(Stage* stage)
{
    // 新しいstageを設定
    nextStage = stage;
}
