#include "StageManager.h"

// �X�V����
void StageManager::Update()
{
    if (nextStage != nullptr)
    {
        // �Â�stage���I��
        Clear();

        // �V����stage��ݒ�
        currentStage = nextStage;
        nextStage = nullptr;

        // stage����������
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

// �`�揈��
void StageManager::Render(float elapsedTime)
{
    if (currentStage != nullptr)
    {
        currentStage->Render(elapsedTime);
    }
}

// stage�N���A
void StageManager::Clear()
{
    if (currentStage != nullptr)
    {
        currentStage->Finalize();
        delete currentStage;
        currentStage = nullptr;
    }
}

// stage�؂�ւ�
void StageManager::ChangeStage(Stage* stage)
{
    // �V����stage��ݒ�
    nextStage = stage;
}
