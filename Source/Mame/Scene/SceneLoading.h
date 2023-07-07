#pragma once

#include "BaseScene.h"

#include <thread>

#include "../../Game/spriteDissolve.h"

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

    struct Animation
    {
        DirectX::XMFLOAT2 position = {};
        DirectX::XMFLOAT2 size = {};
        DirectX::XMFLOAT2 texPos = {};
        DirectX::XMFLOAT2 texSize = {};
        int animationTime = 0;
        int animationFrame = 0;

        void PlayAnimation(const int& frameTime, const int& totalAnimationFrame);

    }anime;

    int animationSpeed = 10.0f;

private:
    std::unique_ptr<SpriteDissolve> spriteDissolve = nullptr;
    std::unique_ptr<Sprite> spriteLoadMameo = nullptr;

    BaseScene*      nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};

