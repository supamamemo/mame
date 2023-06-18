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
    void Update(const float& elapsedTime)override;      // �X�V����
    void End()override;         // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime)override;     // �`�揈��
    void DrawDebug()override;

private:
    std::unique_ptr<Player> player;
    std::unique_ptr<Boss> boss;


    // ����boss�N���X�ŕ\�����Ă�
    std::unique_ptr<Boss> stage[10];

};

