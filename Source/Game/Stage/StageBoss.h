#pragma once

#include "Stage.h"

#include "../PlayerManager.h"
#include "../Boss.h"

#include "../CannonBall.h"

#include "../Terrain/Terrain.h"

class StageBoss : public Stage
{
public:
    StageBoss();
    ~StageBoss() override {}

    void Initialize()                     override;  // ������
    void Finalize()                       override;  // �I������
    void Begin()                          override;  // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override;  // �X�V����
    void End()                            override;  // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override;  // �`�揈��
    void DrawDebug()                      override;  // �f�o�b�O�`��

private:
    static const int TERRAIN_COUNT = 3;

    //std::unique_ptr<Boss> boss;

    // ����boss�N���X�ŕ\�����Ă�
    //std::unique_ptr<Boss> stage[STAGE_COUNT];
    //std::unique_ptr<Terrain> terrain[TERRAIN_COUNT];
};

