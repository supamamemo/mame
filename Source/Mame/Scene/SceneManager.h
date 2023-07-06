#pragma once
#include "BaseScene.h"

#include "../../Game/Pose.h"


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

        void Initialize();
        void Update(float elapesdTime);  // 更新処理
        void Render(float elapsedTime);  // 描画処理

        void DrawDebug();

        void Clear();   // シーンクリア



        // シーン切り替え
        void ChangeScene(BaseScene* scene);

        BaseScene* GetCurrentScene()const { return currentScene; }


        // ポーズ画面
        void PoseUpdateTitle(float elapsedTime);
        void PoseRenderTitle(float elapsedTime);
        void PoseUpdateGame(float elapsedTime);
        void PoseRenderGame(float elapsedTime);
        void SetPose(bool p) { pose = p; }
        bool GetPose() { return pose; }

        void PlayHitStop() 
        {
            hitStopTimer_ = defaultHitStopTime_;
            isHitStop_    = true;
        }

    public:
        float   defaultHitStopTime_ = 0.25f;
        float   hitStopTimer_       = 0.0f;
        bool    isHitStop_          = false;

    private:
        BaseScene* currentScene = nullptr;
        BaseScene* nextScene = nullptr;

        // pose関連
        bool pose = false;
        std::unique_ptr<Pose>sprite[10]{};
        int RS = 0, DS = 0, SS = 0;

        int state = 0;

    public:
        // ファイル用
        int selectState = 0;
    };
}
