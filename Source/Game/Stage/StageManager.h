#pragma once

#include "Stage.h"

#include <DirectXMath.h>

class StageManager
{
private:
    StageManager()  {}
    ~StageManager() {}

public:
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    void Update(const float& elapsedTime);  // 更新処理
    void Render(const float& elapsedTime);  // 描画処理
    void Clear();                           // クリア
  
    void ChangeStage(Stage* stage);       // ステージ切り替え

    Stage* GetCurrentStage() const { return currentStage; }

public:
    enum class SavedHalfwayPointStage
    {
        None,
        StagePlains,
        StageEx,
    };

    struct SavedHalfPoint
    {
        SavedHalfwayPointStage savedHalfwayPointStage = SavedHalfwayPointStage::None;
        DirectX::XMFLOAT3 position = {};
    };

    SavedHalfPoint savedHalfPoint_ = {};

    //std::unique_ptr<SpriteDissolve> spriteDissolve;

private:
    Stage* currentStage = nullptr;
    Stage* nextStage    = nullptr;

};

