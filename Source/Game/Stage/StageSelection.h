#pragma once
#include "Stage.h"

#include "../Box.h"

class StageSelection : public Stage
{
public:
    StageSelection();
    ~StageSelection()override {}

    void Initialize()                       override;   // ������
    void Finalize()                         override;   // �I������
    void Begin()                            override;   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override;   // �X�V����
    void End()                              override;   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime)   override;   // �`�揈��
    void DrawDebug()                        override;   // �f�o�b�O�`��

    void PointUpdate();
    void PointRender(const float& elapsedTime);

private:
    int selectState = 0;

private:
    std::unique_ptr<Box>back = nullptr;
    std::unique_ptr<Box>castle = nullptr;
    std::unique_ptr<Box>boss = nullptr;
    std::unique_ptr<Box>Ex = nullptr;
    std::unique_ptr<Box>signBoard = nullptr;

    std::unique_ptr<Box>point[3];

private:
    enum POINT
    {
        Black,
        Red,
        Blue,
    };

    enum POINTSTATE
    {
        Tutorial,
        Plains,
        Boss,
        EX,
        Clear,
    };

    enum SELECT
    {
        TutorialStage,  // �`���[�g���A��
        Move_T_P,       // �`���[�g���A��->�쌴
        Move_P_T,       // �쌴->�`���[�g���A��
        PlainsStage,    // �쌴
        Move_P_B,       // �쌴->�{�X
        Move_B_P,       // �{�X->�쌴
        BossStage,      // �{�X

        Move_B_E,   // �{�X->Ex
        Move_E_B,   // Ex->�{�X
        ExStage,    // Ex

    };

};

