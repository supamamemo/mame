#pragma once

#include <string>
#include "../Mame/Graphics/Model.h"


class Box
{
public:
    Box(const char* filename);
    ~Box() {}

    void Initialize();                        // 初期化
    void Finalize();                          // 終了化
    void Begin() {}                             // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);    // 更新処理
    void End() {}                               // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime);    // 描画処理
    void DrawDebug();                         // デバッグ描画

public:
    Transform* GetTransform() { return model->GetTransform(); }
    void SetMaterialColor(DirectX::XMFLOAT4 color) { materialColor = color; }
    DirectX::XMFLOAT4 GetMaterialColor() { return materialColor; }

    static float renderLengthXLimit_;   // boxを描画する距離制限

public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //
private:
    std::string name;   // Imgui用
    std::unique_ptr<Model>model = nullptr;

    DirectX::XMFLOAT4 materialColor = { 1.0f, 1.0f, 1.0f, 1.0f };
};

