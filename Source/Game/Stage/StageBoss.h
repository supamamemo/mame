#pragma once

#include "Stage.h"

#include "../PlayerManager.h"
#include "../Boss.h"

#include "../CannonBall.h"


#include "../EnemyTofu.h"
#include "../Terrain/Terrain.h"

#include "../../sprite.h"

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
    //static const int TERRAIN_COUNT = 3;

    // ����boss�N���X�ŕ\�����Ă�
    std::unique_ptr<Boss> back;

    std::unique_ptr<EnemyTofu> tofu;

    std::unique_ptr<Sprite> chefHat = nullptr;

    struct SpriteStruct
    {
        DirectX::XMFLOAT2 pos{ 440,10 };
        DirectX::XMFLOAT2 texPos{ 100,100 };
    }spr;

};

