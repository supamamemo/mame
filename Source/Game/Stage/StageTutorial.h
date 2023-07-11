#pragma once
#include "Stage.h"

#include "../../Mame/Graphics/Effect.h"

#include "../Box.h"
#include "../SpriteAnimation.h"


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
        Batu,
        Maru,
        Setumei,
    };

    enum STATE
    {
        MoveReception,  // 移動受付
        JumpReception,  // ジャンプ受付
        HipDorop,
        Run,
        Free,
        GoalState,
        Start,
    };

    enum TutorialTerrain
    {
        Zimen0,
        Zimen1,
        Zimen2,
        Zimen3,
        Zimen4,
        Zimen5,
        Zimen6,
        Zimen7,
        Zimen8,
        SignBoard_yazi0,
        SignBoard_drop0,
        SignBoard_drop1,
        SignBoard_dash0,
        SignBoard_noTex0,
        SignBoard_noTex1,
        //Goal,
    };

    void TutorialStateUpdate(float elapsedTime);
    void TutorialStateRender(float elapsedTime);

    void StickState(float elapsedTime);
    void StickStateDown(float elapsedTime);
   
    void StickAnimation(float elapsedTime, int state);

    void MarkUpdate(float elapsedTime);
    int markState = 0;
    float markTimer = 0.0f;

    float setumeiTimer = 0.0f;

private:
    std::unique_ptr<Box> back = nullptr;
    std::unique_ptr<Box> goal_ = nullptr;

    std::unique_ptr<SpriteAnimation> spriteAnimation[2];

    float frameSpeed = 8.0f;    // フレーム速度
    float stickTime = 0;
    int tutorialState = 0;
    int stickMoveState = 0; // スティック動き
    

    int signBoardState = 0;

    bool isPlayerMove = false;
    bool isDisplaySignBoard[2] = {};

    // UI
    Effect* effect[4] = {};
};

