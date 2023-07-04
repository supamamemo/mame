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

public:
    void PlayerHpUiUpdate(float elapsedTime);    // �v���C���[hpUI�Ǘ�
    void UpdateUi(int uiCount, float speed, int state);

    int uiState = -1;
    void SetUiState() { uiState = 0; }

    DirectX::XMFLOAT2 GetPlayerUiPosition() { return playerUiPos; }
    void SetPlayerUiPosition(DirectX::XMFLOAT2 pos) { playerUiPos = pos; }
    DirectX::XMFLOAT2 GetPlayerUiSize() { return playerUiSize; }
    void SetPlayerUiSize(DirectX::XMFLOAT2 size) { playerUiSize = size; }

    DirectX::XMFLOAT2 playerUiPos = { 10, 10 };
    DirectX::XMFLOAT2 playerUiSize = { 344, 160 };

private:
    enum UISPRITE
    {
        BaseMameHp,
        mameHpLeft,
        mameHpCenter,
        mameHpRight,
    };
};

