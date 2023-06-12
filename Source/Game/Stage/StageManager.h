#pragma once

#include "Stage.h"

class StageManager
{
private:
    StageManager() {}
    ~StageManager() {}

public:
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    void Update(float elapsedTime);  // 更新処理
    void Render(float elapsedTime); // 描画処理
    void Clear();   // クリア

    // ステージ切り替え
    void ChangeStage(Stage* stage);

    Stage* GetCurrentStage()const { return currentStage; }

private:
    Stage* currentStage = nullptr;
    Stage* nextStage = nullptr;
};

