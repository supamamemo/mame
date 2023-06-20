#include "StageManager.h"
#include "../Common.h"

// 更新処理
void StageManager::Update(const float& elapsedTime)
{
    if (nextStage != nullptr)
    {
        // 古いstageを終了
        Clear();

        // 新しいstageを設定
        currentStage = nextStage;
        nextStage    = nullptr;

        // stage初期化処理
        currentStage->Initialize();
    }

    if (currentStage != nullptr)
    {
        currentStage->Begin();
        currentStage->Update(elapsedTime);
#ifdef USE_IMGUI
        currentStage->DrawDebug();
#endif
        currentStage->End();
    }
}

// 描画処理
void StageManager::Render(const float& elapsedTime)
{
    if (currentStage != nullptr)
    {
        currentStage->Render(elapsedTime);
    }
}

// stageクリア
void StageManager::Clear()
{
    currentStage->Finalize();
    SafeDelete(currentStage);
}

// stage切り替え
void StageManager::ChangeStage(Stage* stage)
{
    // 新しいstageを設定
    nextStage = stage;
}
