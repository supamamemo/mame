#pragma once
#include "Stage.h"

#include "../Box.h"

#include "../../Mame/Graphics/Effect.h"

// チュートリアルステージ
class StageTutorial : public Stage
{
public:
    StageTutorial();
    ~StageTutorial()override;

    void Initialize()                     override;  // 初期化
    void Finalize()                       override;  // 終了処理
    void Begin()                          override;  // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override;  // 更新処理
    void End()                            override;  // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override;  // 描画処理
    void DrawDebug()                      override;  // デバッグ描画


private:
    enum UISPRITE
    {
        Bubble,         // 吹き出し
        BubbleBig,      // 大きい吹き出し
        BubbleBigBig,   // さらに大きい吹き出し
        BubbleStick,    // 吹き出しスティック
        StickBase,      // base
        StickCenter,    // center
        StickRight,     // right
        StickLeft,      // left
        StickDown,
        GamePadA,
        GamePadB,
        GamePadX,
        GamePadY,
        KeyBoardA,
        KeyBoardD,
        KeyBoardS,
        KeyBoardSHIFT,
        KeyBoardSPACE,
        SpriteOR,       // or
        Plus,           // +
    };

    enum STATE
    {
        MoveReception,  // 移動受付
        JumpReception,  // ジャンプ受付
        HipDorop,
        Run,
        Free,
    };

    enum tutorialTerrain
    {
        Zimen0,
        Zimen1,
        Zimen2,
        Zimen3,
        Zimen4,
        Zimen5,
        Zimen6,
        Zimen7,
        SignBoard_yazi0,
        SignBoard_drop0,
        SignBoard_dash0,
    };

    void TutorialStateUpdate(float elapsedTime);
    void TutorialStateRender(float elapsedTime);

    int tutorialState = 0;

    

private:
    int stickMoveState = 0; // スティック動き
    float stickTime = 0;
    void StickState(float elapsedTime);
    void StickStateDown(float elapsedTime);

private:
    std::unique_ptr<Box> back = nullptr;
    //std::unique_ptr<Box> signboard1 = nullptr;

    // UI
    Effect* effect[4];
};

