#pragma once

#include "Stage.h"

#include "../PlayerManager.h"
#include "../Boss.h"
#include "../Box.h"

#include "../CannonBall.h"


#include "../EnemyTofu.h"
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
    std::unique_ptr<Box> back;

    float   clearTimer_     = 0.0f;
    bool    isEnemyUpdate_  = false;

public:
    // UI�֘A�i�}���ō���Ă邩��R�����g�Ȃ��ł��߂�B�B�j
    void PlayerHpUiUpdate(float elapsedTime);    // �v���C���[hpUI�Ǘ�
    void PlayerUpdateUi(int uiCount, float speed, int state, float elapsedTime);

    void BossHpUiUpdate(float elapsedTime);
    void BossUpdateUi(int uiCount, float speed, int state, float elapsedTime);

    int uiState = -1;
    void SetUiState() { uiState = 0; }

    DirectX::XMFLOAT3 GetPlayerUiPosition() { return playerUiPos; }
    void SetPlayerUiPosition(DirectX::XMFLOAT3 pos) { playerUiPos = pos; }
    DirectX::XMFLOAT2 GetPlayerUiSize() { return playerUiSize; }
    void SetPlayerUiSize(DirectX::XMFLOAT2 size) { playerUiSize = size; }

    DirectX::XMFLOAT3 playerUiPos = { 10, 10 ,10 };
    DirectX::XMFLOAT2 playerUiSize = { 344, 160 };

    DirectX::XMFLOAT3 bossUiPos = { 815,30,10 };
    DirectX::XMFLOAT2 bossUiSize = { 440,160 };

    void subtractUiTimer(float time) { uiTimer -= time; }
    void SetUiTimer(float time) { uiTimer = time; }
    float GetUiTimer() { return uiTimer; }
    float uiTimer = 0.0f;

    DirectX::XMFLOAT3 displayUiPosition{};

private:
    enum UISPRITE
    {
        BaseMameHp,
        mameHpLeft,
        mameHpCenter,
        mameHpRight,
        BaseBossHp,
        BossHpLeft,
        BossHpCenter,
        BossHpRight,
    };

};

