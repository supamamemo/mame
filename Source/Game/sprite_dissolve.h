#pragma once

#include "../sprite.h"
#include <memory>

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

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

    // load用
    void fadeIn(float elapsedTime);
    void fadeOut(float elapsedTime);
    void SetFadeInTexture(XMFLOAT2 position, XMFLOAT2 textureSize, float delay, int textureType)
    {
        spr_dissolve.pos                = position;
        spr_dissolve.posD               = textureSize;
        spr_dissolve.delay              = delay;
        spr_dissolve.mask_texture_value = textureType;
        spr_dissolve.dissolve_value1    = 1.5f;
    }
    void SetFadeOutTexture(XMFLOAT2 position, XMFLOAT2 textureSize, float delay, int textureType)
    {
        spr_dissolve.pos                = position;
        spr_dissolve.posD               = textureSize;
        spr_dissolve.delay              = delay;
        spr_dissolve.mask_texture_value = textureType;
    }
    bool fadeInReady(float time) { return spr_dissolve.dissolve_value1 < time; }
    bool fadeOutReady(float time) { return spr_dissolve.dissolve_value1 > time; }

public:
    // デバッグ用
    bool debug_initialize = false;

private:
    struct dissolve_constants
    {
        XMFLOAT4 parameters = {};   // x:ディゾルブ適応量、
                                    // y: 黒色
                                    // z: 縁の閾値
                                    // w:空き
        XMFLOAT4 edgeColor  = {};   // 縁の色
    };

    struct SprDissolve
    {
        XMFLOAT2 pos                = { 600.0f, 420.0f };
        XMFLOAT2 posD               = { 300.0f, 300.0f };

        int      mask_texture_value = 0;
        float    dissolve_value     = 0.0f;
        float    dissolve_value1    = 0.0f;

        // mask edge
        float    edge_threshold     = 0.2f;        // 縁の閾値
        XMFLOAT4 edgeColor          = { 1,0,0,1 }; // 縁の色

        int      dissolve_state     = 0;           // dissolveの種類
        float    delay              = 0.4f;        // fadeのいい感じにするための変数

    } spr_dissolve = {};

private:
    unique_ptr<sprite>               dissolve_sprite     = nullptr;
    ComPtr<ID3D11SamplerState>       samplerState        = nullptr;

    D3D11_TEXTURE2D_DESC             mask_texture2d_desc = {};
    ComPtr<ID3D11ShaderResourceView> mask_texture[20]    = {};

    ComPtr<ID3D11Buffer>             dissolve_constant_buffer = nullptr;
};

