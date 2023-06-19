#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>

class Sprite
{
public:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;  // 頂点シェーダーインターフェイス
    // 頂点シェーダーインターフェイスは、頂点シェーダーステージを制御する
    // 実行可能プログラム(頂点シェーダー)を管理します。
    // <頂点シェーダー> 頂点データを受け取ってスクリーン座標に変換する計算をする
    // <頂点データ> 座標、色などのポリゴンを構成する為に必要な情報

    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;    // ピクセルシェーダーインターフェイス
    // ピクセルシェーダーインターフェイスは、ピクセルシェーダーステージを制御する
    // 実行可能プログラム(ピクセルシェーダー)を管理します
    // <ピクセルシェーダー> ポリゴンの色をピクセル単位で計算する

    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;    // 入力レイアウトインターフェイス
    // 入力レイアウトインターフェイスは、メモリ内にレイアウトされている頂点データを
    // グラフィックスパイプラインの入力アセンブラーステージにフィードする方法の定義を保持します
    // <入力レイアウト> 頂点データにどのような情報があるのか定義し、シェーダーに教えるやつ

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;        // バッファーインターフェイス
    // バッファーインターフェイスは、非構造化メモリであるバッファーリソースにアクセスします
    // バッファーは通常、頂点データまたはインデックスデータを格納します
    // <頂点バッファ> 頂点データをGPUで扱う為の格納用バッファ

    // <描画ステート> ポリゴンの表示方法や、奥行比較方法などの様々な設定をする
    //      今回はラスタライザステートを設定する

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view; // シェーダーリソースビューインターフェイス
    D3D11_TEXTURE2D_DESC texture2d_desc;

public:
    Sprite(ID3D11Device* device, const wchar_t* filename);
    ~Sprite();
    
    void render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle/*degree*/);
    void render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle/*degree*/, float sx, float sy, float sw, float sh);
    void render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh);

    ID3D11VertexShader* GetVertexShader() { return vertex_shader.Get(); }
    ID3D11PixelShader* GetPixelShader() { return pixel_shader.Get(); }
    ID3D11InputLayout* GetInputLayout() { return input_layout.Get(); }
    ID3D11VertexShader** GetVertexShaderAddress() { return vertex_shader.GetAddressOf(); }
    ID3D11PixelShader** GetPixelShaderAddress() { return pixel_shader.GetAddressOf(); }
    ID3D11InputLayout** GetInputLayoutAddress() { return input_layout.GetAddressOf(); }

    // 頂点フォーマット
    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcord;
    };
};