#include "StageManager.h"
#include "../Common.h"

// �X�V����
void StageManager::Update(const float& elapsedTime)
{
    if (nextStage != nullptr)
    {
        // �Â�stage���I��
        Clear();

        // �V����stage��ݒ�
        currentStage = nextStage;
        nextStage    = nullptr;

        // stage����������
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

// �`�揈��
void StageManager::Render(const float& elapsedTime)
{
    if (currentStage != nullptr)
    {
        currentStage->Render(elapsedTime);
    }
}

// stage�N���A
void StageManager::Clear()
{
    currentStage->Finalize();
    SafeDelete(currentStage);
}

// stage�؂�ւ�
void StageManager::ChangeStage(Stage* stage)
{
    // �V����stage��ݒ�
    nextStage = stage;
}
