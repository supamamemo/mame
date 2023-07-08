#pragma once
#include <string>
#include <vector>

namespace Mame::Scene
{
    enum class TYPE
    {
        TITLE,
        LOAD,
        GAME,
    };

    enum class STAGE
    {
        Tutorial,
        Plains,
        Boss,
        Select,
    };

    class BaseScene
    {
    public:
        BaseScene() {}
        virtual ~BaseScene() {}

        virtual void Initialize()   = 0;                    // ������
        virtual void Finalize()     = 0;                    // �I����
        virtual void Begin()        = 0;                    // ���t���[����ԍŏ��ɌĂ΂��
        virtual void Update(const float& elapsedTime) = 0;  // �X�V����
        virtual void End()          = 0;                    // ���t���[����ԍŌ�ɌĂ΂��
        virtual void Render(const float& elapsedTime) = 0;  // �`�揈��
        virtual void DrawDebug()    = 0;

    public:
        // �����������Ă��邩�ǂ���
        bool IsReady()const { return ready; }

        // ��������
        void SetReady() { ready = true; }

        // ���ǂ̃V�[���Ȃ̂�����ł���悤�ɂ���ׂ̂���
        void SetSceneType(int type) { sceneType = type; }
        int GetSceneType() { return sceneType; }



    private:
        bool ready = false;

        int sceneType = static_cast<int>(TYPE::TITLE);
    };
}
