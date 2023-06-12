#pragma once
#include "BaseScene.h"

namespace Mame::Scene
{
    class SceneManager
    {
    private:
        SceneManager() {}
        ~SceneManager() {}

    public:
        static SceneManager& Instance()
        {
            static SceneManager instance;
            return instance;
        }

        void Update(float elapesdTime);  // �X�V����
        void Render(float elapsedTime);  // �`�揈��
        void Clear();   // �V�[���N���A



        // �V�[���؂�ւ�
        void ChangeScene(BaseScene* scene);

        BaseScene* GetCurrentScene()const { return currentScene; }

    private:
        BaseScene* currentScene = nullptr;
        BaseScene* nextScene = nullptr;
    };
}
