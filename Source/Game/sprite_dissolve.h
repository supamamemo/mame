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

    void Initialize(int state, const wchar_t* filename);
    void Update();
    void Render();
    void DrawDebug(const wchar_t* filename);

    // デバッグ用
    bool debug_initialize = false;

    // load用
    void fadeOut(float elapsedTime);
    void SetFadeOutTexture(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 texSize, float delay, int textureType)
    {
        spr_dissolve.pos = pos;
        spr_dissolve.posD = texSize;
        spr_dissolve.delay = delay;
        spr_dissolve.mask_texture_value = textureType;
    }
    bool fadeOutReady(float time) { return spr_dissolve.dissolve_value1 > time; }

private:
    std::unique_ptr<sprite> dissolve_sprite;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>  samplerState;

    D3D11_TEXTURE2D_DESC mask_texture2d_desc;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mask_texture[20];
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

