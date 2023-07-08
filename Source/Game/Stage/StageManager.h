#pragma once

#include "Stage.h"

class StageManager
{
private:
    StageManager()  {}
    ~StageManager() {}

public:
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    void Update(const float& elapsedTime);  // �X�V����
    void Render(const float& elapsedTime);  // �`�揈��
    void Clear();                           // �N���A
  
    void ChangeStage(Stage* stage);       // �X�e�[�W�؂�ւ�

    Stage* GetCurrentStage() const { return currentStage; }

private:
    Stage* currentStage = nullptr;
    Stage* nextStage    = nullptr;
};

