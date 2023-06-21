#pragma once
#include "Character.h"

class CannonBallManager;

class CannonBall : public Character
{
public:
    CannonBall(DirectX::XMFLOAT3 newPosition, float directionZ, CannonBallManager* manager);
    ~CannonBall();

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Begin();                           // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);  // 更新処理
    void End();                             // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime);  // 描画処理

    void DrawDebug() override;              // デバッグ描画

public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //    


    void Destroy();

    CannonBallManager* manager = nullptr;

private:
    std::string name;   // Imgui用

    int directionZ = 0;
    float offsetZ = 3;
};

