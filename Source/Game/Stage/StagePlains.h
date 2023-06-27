#pragma once

#include "Stage.h"

#include "../PlayerManager.h"
#include "../GrassBlock.h"

class StagePlains : public Stage
{
public:
    StagePlains();
    ~StagePlains() override {}

    void Initialize() override;                     // ������
    void Finalize() override;                       // �I������
    void Begin() override;                          // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End() override;                            // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug() override;

private:
    static const int TERRAIN_COUNT = 26;

    std::unique_ptr<GrassBlock> grassBlock[TERRAIN_COUNT];
};

