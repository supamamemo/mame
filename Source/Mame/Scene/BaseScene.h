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

        virtual void Initialize() = 0;      // 初期化
        virtual void Finalize() = 0;        // 終了化
        virtual void Begin() = 0;           // 毎フレーム一番最初に呼ばれる
        virtual void Update(float elapesdTime) = 0;          // 更新処理
        virtual void End() = 0;             // 毎フレーム一番最後に呼ばれる
        virtual void Render(float elapsedTime) = 0;          // 描画処理
        virtual void DrawDebug() = 0;

    public:
        // 準備完了しているかどうか
        bool IsReady()const { return ready; }

        // 準備完了
        void SetReady() { ready = true; }

    private:
        bool ready = false;
    };
}
