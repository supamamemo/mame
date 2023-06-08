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

    void Update();  // 更新処理
    void Render(float elapsedTime); // 描画処理
    void Clear();   // クリア

    void ChangeStage(Stage* stage);

    Stage* GetCurrentStage()const { return currentStage; }

private:
    Stage* currentStage = nullptr;
    Stage* nextStage = nullptr;
};

