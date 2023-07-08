#pragma once

#include "Stage.h"

#include <thread>

#include "../spriteDissolve.h"
#include "../SpriteAnimation.h"

class StageLoading : public Stage
{
public:
    StageLoading(Stage* nextStage);
    ~StageLoading() override {}

    void Initialize()   override;                   // ������
    void Finalize()     override;                   // �I����
    void Begin()        override;                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()          override;                   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug()    override;

private:
    // ���[�f�B���O�X���b�h
    static void LoadingThread(StageLoading* stage);


private:
    std::unique_ptr<SpriteDissolve> spriteDissolve = nullptr;

    std::unique_ptr<SpriteAnimation> spriteAnimation = nullptr;

    Stage* nextStage = nullptr;
    std::thread* thread = nullptr;
};

