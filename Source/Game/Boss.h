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
    //Boss(const char* filename); // 仮です

    ~Boss()override;

    void Initialize();              // 初期化
    void Finalize();                // 終了化
    void Begin();                   // 毎フレーム一番最初に呼ばれる
    void Update(float elapsedTime); // 更新処理
    void End();                     // 毎フレーム一番最後に呼ばれる
    void Render(float elapsedTime); // 描画処理
    void DrawDebug()override;



public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //    


private:
    std::string name;   // Imgui用
};

