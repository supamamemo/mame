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

    void PointRender(const float& elapsedTime);

private:
    std::unique_ptr<Box>back = nullptr;
    std::unique_ptr<Box>castle = nullptr;
    std::unique_ptr<Box>boss = nullptr;

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
    };

};

