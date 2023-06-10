#pragma once

#include "BaseScene.h"

#include <thread>

#include "../../Game/sprite_dissolve.h"

class SceneLoading :
    public Mame::Scene::BaseScene
{
public:
    SceneLoading(BaseScene* nextScene) :nextScene(nextScene) {}
    ~SceneLoading() override {}

    // ������
    void Initialize()override;

    // �I����
    void Finalize()override;

    void Begin()override;

    // �X�V����
    void Update(float elapesdTime)override;

    void End()override;

    // �`�揈��
    void Render(float elapsedTime)override;

    void DrawDebug()override;

private:
    // ���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene);

private:
    Sprite_dissolve spriteDissolve;

    BaseScene* nextScene = nullptr;
    std::thread* thread = nullptr;
};

