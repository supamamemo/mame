#pragma once
#include "Stage.h"

#include "../Box.h"

#include "../../Mame/Graphics/Effect.h"

// �`���[�g���A���X�e�[�W
class StageTutorial : public Stage
{
public:
    StageTutorial();
    ~StageTutorial()override;

    void Initialize()                     override;  // ������
    void Finalize()                       override;  // �I������
    void Begin()                          override;  // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override;  // �X�V����
    void End()                            override;  // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override;  // �`�揈��
    void DrawDebug()                      override;  // �f�o�b�O�`��

private:
    void TutorialStateUpdate(float elapsedTime);
    void TutorialStateRender(float elapsedTime);

    int tutorialState = 0;
    

    enum STATE
    {
        MoveReception,  // �ړ���t
        JumpReception,  // �W�����v��t
    };

private:
    int stickMoveState = 0; // �X�e�B�b�N����
    float stickTime = 0;
    void StickState(float elapsedTime);

private:
    // ����boss�N���X�ŕ\�����Ă���
    std::unique_ptr<Box> back = nullptr;
    std::unique_ptr<Box> box = nullptr;

    // UI

    Effect* effect = nullptr;
};

