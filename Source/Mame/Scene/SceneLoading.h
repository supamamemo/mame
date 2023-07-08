#pragma once

#include "BaseScene.h"

#include <thread>

#include "../../Game/spriteDissolve.h"

#include "../../Game/SpriteAnimation.h"

class SceneLoading : public Mame::Scene::BaseScene
{
public:
    SceneLoading(BaseScene* nextScene);
    ~SceneLoading() override {}


    void Initialize()   override;                   // ������
    void Finalize()     override;                   // �I����
    void Begin()        override;                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()          override;                   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug()    override;

private:
    // ���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene);


private:
    std::unique_ptr<SpriteDissolve> spriteDissolve = nullptr;

    std::unique_ptr<SpriteAnimation> spriteAnimation = nullptr;

    BaseScene*      nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};

