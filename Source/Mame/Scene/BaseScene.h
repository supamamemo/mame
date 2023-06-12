#pragma once
#include <string>
#include <vector>

namespace Mame::Scene
{
    class BaseScene
    {
    public:
        BaseScene() {}
        virtual ~BaseScene() {}

        virtual void Initialize() = 0;      // ������
        virtual void Finalize() = 0;        // �I����
        virtual void Begin() = 0;           // ���t���[����ԍŏ��ɌĂ΂��
        virtual void Update(float elapesdTime) = 0;          // �X�V����
        virtual void End() = 0;             // ���t���[����ԍŌ�ɌĂ΂��
        virtual void Render(float elapsedTime) = 0;          // �`�揈��
        virtual void DrawDebug() = 0;

    public:
        // �����������Ă��邩�ǂ���
        bool IsReady()const { return ready; }

        // ��������
        void SetReady() { ready = true; }

    private:
        bool ready = false;
    };
}
