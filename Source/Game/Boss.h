#pragma once

#include "Enemy.h"

enum class BossAnimation
{
    Idle,
    Find,
    Attack,
    Recoil,
};

class Boss : public Enemy
{
public:
    Boss();
    Boss(const char* filename); // 仮です

    ~Boss() override;

    void Initialize()                     override;   // 初期化
    void Finalize()                       override;   // 終了化
    void Begin()                          override;   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override;   // 更新処理
    void End()                            override;   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override;   // 描画処理
    void DrawDebug()                      override;   // デバッグ描画

    void OnDead()                         override;   // 死亡したときに呼ばれる

public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //    


private:
    std::string name;   // Imgui用
};

