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

        virtual void Initialize()   = 0;                    // 初期化
        virtual void Finalize()     = 0;                    // 終了化
        virtual void Begin()        = 0;                    // 毎フレーム一番最初に呼ばれる
        virtual void Update(const float& elapsedTime) = 0;  // 更新処理
        virtual void End()          = 0;                    // 毎フレーム一番最後に呼ばれる
        virtual void Render(const float& elapsedTime) = 0;  // 描画処理
        virtual void DrawDebug()    = 0;

    public:
        // 準備完了しているかどうか
        bool IsReady()const { return ready; }

        // 準備完了
        void SetReady() { ready = true; }

        // 今どのシーンなのか判定できるようにする為のもの
        void SetSceneType(int type) { sceneType = type; }
        int GetSceneType() { return sceneType; }



    private:
        bool ready = false;

        int sceneType = static_cast<int>(TYPE::TITLE);


    public:
        // stageを変更するいい方法が思いつかなかったので、とりあえずここで。。。
        void ChangeStage(int stage);
        bool GetChangeStageTutorial() { return changeStageTutorial; }
        bool GetChangeStagePlains() { return changeStagePlains; }
        bool GetChangeStageBoss() { return changeStageBoss; }
        bool GetChangeStageSelect() { return changeStageSelect; }
        void SetChangeStageTutorial() { changeStageTutorial = false; }
        void SetChangeStagePlains() { changeStagePlains = false; }
        void SetChangeStageBoss() { changeStageBoss = false; }
        void SetChangeStageSelect() { changeStageSelect = false; }


    private:
        // stageを変更するいい方法が思いつかなかったので、とりあえずここで。。。
        bool changeStageTutorial = false;
        bool changeStagePlains = false;
        bool changeStageBoss = false;
        bool changeStageSelect = false;

    };
}
