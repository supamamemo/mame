#pragma once

#include "Stage.h"

class StagePlains : public Stage
{
public:
    StagePlains() {}
    ~StagePlains()override {}

    void Initialize()override;  // ������
    void Finalize()override;    // �I������
    void Begin()override;       // ���t���[����ԍŏ��ɌĂ΂��
    void Update()override;      // �X�V����
    void End()override;         // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime)override;     // �`�揈��
    void DrawDebug()override;
};
