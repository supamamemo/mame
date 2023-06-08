#pragma once

#include "../sprite.h"
#include <memory>

enum class Dissolve
{
    Fire,
    Fade,
};

class Sprite_dissolve
{
public:
    Sprite_dissolve() {}
    ~Sprite_dissolve() {}

    void Initialize(int state);
    void Update();
    void Render();
    void DrawDebug();

    // デバッグ用
    void DebugInitialize();
    bool debug_initialize = false;

private:
    std::unique_ptr<sprite> dissolve_sprite;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>  samplerState;

    D3D11_TEXTURE2D_DESC mask_texture2d_desc;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mask_texture[10];
    struct dissolve_constants
    {
        DirectX::XMFLOAT4 parameters;   // x:ディゾルブ適応量、
                                        // y: 黒色
                                        // z: 縁の閾値
                                        // w:空き
        DirectX::XMFLOAT4 edgeColor;    // 縁の色
    };

    struct SprDissolve
    {
        DirectX::XMFLOAT2 pos{ 600.0f,420.0f };
        DirectX::XMFLOAT2 posD{ 300.0f,300.0f };

        int mask_texture_value = 0;
        float dissolve_value{ 0.0f };
        float dissolve_value1{ 0.0f };

        // mask edge
        float edge_threshold{ 0.2f };   // 縁の閾値
        DirectX::XMFLOAT4 edgeColor{ 1,0,0,1 };    // 縁の色
        
        int dissolve_state{};   // dissolveの種類
        float delay = 0.4f; // fadeのいい感じにするための変数

    }spr_dissolve;
    Microsoft::WRL::ComPtr<ID3D11Buffer> dissolve_constant_buffer;



};

