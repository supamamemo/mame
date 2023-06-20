#pragma once
#include "BaseScene.h"

#include "../../Game/spriteDissolve.h"

class SceneGame : public Mame::Scene::BaseScene
{
public:
    SceneGame();
    ~SceneGame()override {}

    void Initialize()   override;                   // ������
    void Finalize()     override;                   // �I����
    void Begin()        override;                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()          override;                   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug()    override;

public:
    // fadeOut/fadeIn�p�̃f�B�]���u
    std::unique_ptr<SpriteDissolve> spriteDissolve;

    // �f�o�b�O�p
    int fadeType = 1;
    int textureType = 0;

};

