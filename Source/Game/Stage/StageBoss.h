#pragma once

#include "Stage.h"

#include "../Player.h"
#include "../Boss.h"

class StageBoss : public Stage
{
public:
    StageBoss();
    ~StageBoss()override {}

    void Initialize()override;  // ������
    void Finalize()override;    // �I������
    void Begin()override;       // ���t���[����ԍŏ��ɌĂ΂��
    void Update()override;      // �X�V����
    void End()override;         // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime)override;     // �`�揈��
    void DrawDebug()override;

private:
    std::unique_ptr<Player> player;
    std::unique_ptr<Boss> boss;

    std::unique_ptr<Model>stage[2];

};

