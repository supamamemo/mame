#pragma once
#include "BaseScene.h"

#include "../../Game/Player.h"

class SceneGame : public Mame::Scene::BaseScene
{
public:
    SceneGame() {}
    ~SceneGame()override {}

    void Initialize()override;      // ������
    void Finalize()override;        // �I����
    void Begin()override;           // ���t���[����ԍŏ��ɌĂ΂��
    void Update()override;          // �X�V����
    void End()override;             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime)override;          // �`�揈��
    void DrawDebug()override;

public:
    Player* player = nullptr;
};

