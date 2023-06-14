#pragma once

#include "Character.h"

class Boss : public Character
{
public:
    Boss();
    Boss(const char* filename); // 仮です

    ~Boss()override;

    void Initialize();      // 初期化
    void Finalize();        // 終了化
    void Begin();           // 毎フレーム一番最初に呼ばれる
    void Update();          // 更新処理
    void End();             // 毎フレーム一番最後に呼ばれる
    void Render(float elapsedTime);          // 描画処理
    void DrawDebug()override;

public:
    static int nameNum;

    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }

private:
    std::string name;
};

