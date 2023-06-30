#pragma once

#include "Stage.h"

#include "../PlayerManager.h"
#include "../GrassBlock.h"

class StagePlains : public Stage
{
public:
    StagePlains();
    ~StagePlains() override {}

    void Initialize()                       override;   // ������
    void Finalize()                         override;   // �I������
    void Begin()                            override;   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override;   // �X�V����
    void End()                              override;   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime)   override;   // �`�揈��
    void DrawDebug()                        override;   // �f�o�b�O�`��
};

