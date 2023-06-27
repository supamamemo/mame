#pragma once

#include <memory>
#include "../../Mame/Graphics/Model.h"
#include "../../GeometricPrimitive.h"
#include "../Collision.h"

class Terrain
{
public:
    Terrain(const char* const filename);
    virtual ~Terrain() = default;

    virtual void Initialize()                       = 0; // 初期化
    virtual void Finalize()                         = 0; // 終了化
    virtual void Begin()                            = 0; // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime)   = 0; // 更新処理
    virtual void End()                              = 0; // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float& elapsedTime);       // 描画処理
    virtual void DrawDebug();                            // デバッグ描画

public:
    void UpdateAABB();                                   // AABB更新処理

public:
    Transform* GetTransform() const { return model->GetTransform(); }   // トランスフォーム取得

public:
    std::unique_ptr<Model> model;                         // モデル

    std::unique_ptr<GeometricPrimitive> geometricAABB_;   // 当たり判定描画用ジオメトリックプリミティブ
    Collision::AABB aabb_ = {};                           // 当たり判定構造体


    // Imgui名前被り防止のためのやつ //
    const char* const GetName() const { return name.c_str(); }
    void SetName(const char* const n) { name = n; }

public:
    static int nameNum;

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders = {};

    DirectX::XMFLOAT4 materialColor = { 0.0f, 1.0f, 0.0f, 0.4f }; // マテリアルカラー

private:
    // Imgui名前被り防止のためのやつ //    
    std::string name;   // Imgui用

    DirectX::XMFLOAT3 defaultMin_ = {};
    DirectX::XMFLOAT3 defaultMax_ = {};
};