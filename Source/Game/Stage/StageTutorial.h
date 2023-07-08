#pragma once
#include "Stage.h"

#include "../../Mame/Graphics/Effect.h"

#include "../Box.h"
#include "../SpriteAnimation.h"


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
    enum UISPRITE
    {
        Bubble,         // �����o��
        BubbleBig,      // �傫�������o��
        BubbleBigBig,   // ����ɑ傫�������o��
        BubbleStick,    // �����o���X�e�B�b�N
        StickBase,      // base
        StickCenter,    // center
        StickRight,     // right
        StickLeft,      // left
        StickDown,
        GamePadA,
        GamePadB,
        GamePadX,
        GamePadY,
        KeyBoardA,
        KeyBoardD,
        KeyBoardS,
        KeyBoardSHIFT,
        KeyBoardSPACE,
        SpriteOR,       // or
        Plus,           // +
    };

    enum STATE
    {
        MoveReception,  // �ړ���t
        JumpReception,  // �W�����v��t
        HipDorop,
        Run,
        Free,
        GoalState,
    };

    enum TutorialTerrain
    {
        Zimen0,
        Zimen1,
        Zimen2,
        Zimen3,
        Zimen4,
        Zimen5,
        Zimen6,
        Zimen7,
        Zimen8,
        SignBoard_yazi0,
        SignBoard_drop0,
        SignBoard_drop1,
        SignBoard_dash0,
        Goal,
    };

    void TutorialStateUpdate(float elapsedTime);
    void TutorialStateRender(float elapsedTime);

    int tutorialState = 0;

    

private:
    int stickMoveState = 0; // �X�e�B�b�N����
    float stickTime = 0;
    void StickState(float elapsedTime);
    void StickStateDown(float elapsedTime);

private:
    std::unique_ptr<Box> back = nullptr;

    std::unique_ptr<SpriteAnimation> spriteAnimation = nullptr;
    
    float frameSpeed = 8.0f;    // �t���[�����x

    // UI
    Effect* effect[4];
};

