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
    };

    struct SavedHalfPoint
    {
        SavedHalfwayPointStage savedHalfwayPointStage = SavedHalfwayPointStage::None;
        DirectX::XMFLOAT3 position = {};
    };

    SavedHalfPoint savedHalfPoint_ = {};

private:
    Stage* currentStage = nullptr;
    Stage* nextStage    = nullptr;

};

