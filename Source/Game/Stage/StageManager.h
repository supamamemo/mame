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

    void Update();  // �X�V����
    void Render(float elapsedTime); // �`�揈��
    void Clear();   // �N���A

    void ChangeStage(Stage* stage);

    Stage* GetCurrentStage()const { return currentStage; }

private:
    Stage* currentStage = nullptr;
    Stage* nextStage = nullptr;
};

