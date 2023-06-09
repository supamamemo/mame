#pragma once
#include "Stage.h"

#include "../Box.h"

class StageSelection : public Stage
{
public:
    StageSelection();
    ~StageSelection()override {}

    void Initialize()                       override;   // 初期化
    void Finalize()                         override;   // 終了処理
    void Begin()                            override;   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)   override;   // 更新処理
    void End()                              override;   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime)   override;   // 描画処理
    void DrawDebug()                        override;   // デバッグ描画

    void PointUpdate();
    void PointRender(const float& elapsedTime);

private:
    int selectState = 0;

private:
    std::unique_ptr<Box>back = nullptr;
    std::unique_ptr<Box>castle = nullptr;
    std::unique_ptr<Box>boss = nullptr;
    std::unique_ptr<Box>Ex = nullptr;
    std::unique_ptr<Box>signBoard = nullptr;

    std::unique_ptr<Box>point[3];

private:
    enum POINT
    {
        Black,
        Red,
        Blue,
    };

    enum POINTSTATE
    {
        Tutorial,
        Plains,
        Boss,
        EX,
        Clear,
    };

    enum SELECT
    {
        TutorialStage,  // チュートリアル
        Move_T_P,       // チュートリアル->野原
        Move_P_T,       // 野原->チュートリアル
        PlainsStage,    // 野原
        Move_P_B,       // 野原->ボス
        Move_B_P,       // ボス->野原
        BossStage,      // ボス

        Move_B_E,   // ボス->Ex
        Move_E_B,   // Ex->ボス
        ExStage,    // Ex

    };

};

