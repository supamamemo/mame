#pragma once

#include "Character.h"
#include "StateMachine.h"


enum class BossAnimation
{
    Idle,
    Find,
    Attack,
    Recoil,
};

class Boss : public Character
{
public:
    Boss();
    Boss(const char* filename); // 仮です

    ~Boss()override;

    void Initialize();              // 初期化
    void Finalize();                // 終了化
    void Begin();                   // 毎フレーム一番最初に呼ばれる
    void Update(float elapsedTime); // 更新処理
    void End();                     // 毎フレーム一番最後に呼ばれる
    void Render(float elapsedTime); // 描画処理
    void DrawDebug()override;

    // アニメーション再生設定
    void PlayAnimation(const int& index, const bool& loop);

    // アニメーション更新処理
    void UpdateAnimation(const float& elapsedTime);

    // アニメーションが再生中かどうか
    bool IsPlayAnimation() const;

public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //

    // ステートマシン
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

    // これはテスト用で使ってるだけなので、、後でいらなくなるかも
    DirectX::XMFLOAT4 materialColor = { 1,1,1,1 };
    void SetMaterialColor(DirectX::XMFLOAT4 color) { materialColor = color; }

    

private:
    std::string name;   // Imgui用

    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // ステートマシン
};

