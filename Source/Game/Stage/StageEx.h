#pragma once
#include "Stage.h"

#include "../Terrain/TerrainManager.h"
#include "../PlayerManager.h"
#include "../EnemyManager.h"

#include "../Box.h"

class StageEx : public Stage
{
public:
    StageEx();
    ~StageEx()override {}

    void Initialize()                     override;  // ������
    void Finalize()                       override;  // �I������
    void Begin()                          override;  // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override;  // �X�V����
    void End()                            override;  // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override;  // �`�揈��
    void DrawDebug()                      override;  // �f�o�b�O�`��

private:
    void RegisterTerrains(TerrainManager& terrainManager);  // �n�`����
    void RegisterEnemies(EnemyManager& enemyManager);       // �G�l�~�[����

    void SetTerrains(TerrainManager& terrainManager);       // �n�`�ݒ�
    void SetEnemies(EnemyManager& enemyManager);            // �G�l�~�[�ݒ�

public:
    // UI�֘A�i�}���ō���Ă邩��R�����g�Ȃ��ł��߂�B�B�j
    void PlayerHpUiUpdate(float elapsedTime);    // �v���C���[hpUI�Ǘ�
    void UpdateUi(int uiCount, float speed, int state, float elapsedTime);

    int uiState = -1;
    void SetUiState() { uiState = 0; }

    DirectX::XMFLOAT3 GetPlayerUiPosition() { return playerUiPos; }
    void SetPlayerUiPosition(DirectX::XMFLOAT3 pos) { playerUiPos = pos; }
    DirectX::XMFLOAT2 GetPlayerUiSize() { return playerUiSize; }
    void SetPlayerUiSize(DirectX::XMFLOAT2 size) { playerUiSize = size; }

    DirectX::XMFLOAT3 playerUiPos = { 10, 10 ,10 };
    DirectX::XMFLOAT2 playerUiSize = { 344, 160 };

    void subtractUiTimer(float time) { uiTimer -= time; }
    void SetUiTimer(float time) { uiTimer = time; }
    float GetUiTimer() { return uiTimer; }
    float uiTimer = 0.0f;

    DirectX::XMFLOAT3 displayUiPosition{};

private:
    std::unique_ptr<Box> back = nullptr;

private:
    enum UISPRITE
    {
        BaseMameHp,
        mameHpLeft,
        mameHpCenter,
        mameHpRight,
    };
};
