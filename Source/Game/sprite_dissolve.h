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

    // �f�o�b�O�p
    bool debug_initialize = false;

    // load�p
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
        DirectX::XMFLOAT4 parameters;   // x:�f�B�]���u�K���ʁA
                                        // y: ���F
                                        // z: ����臒l
                                        // w:��
        DirectX::XMFLOAT4 edgeColor;    // ���̐F
    };

    struct SprDissolve
    {
        DirectX::XMFLOAT2 pos{ 600.0f,420.0f };
        DirectX::XMFLOAT2 posD{ 300.0f,300.0f };

        int mask_texture_value = 0;
        float dissolve_value{ 0.0f };
        float dissolve_value1{ 0.0f };

        // mask edge
        float edge_threshold{ 0.2f };   // ����臒l
        DirectX::XMFLOAT4 edgeColor{ 1,0,0,1 };    // ���̐F
        
        int dissolve_state{};   // dissolve�̎��
        float delay = 0.4f; // fade�̂��������ɂ��邽�߂̕ϐ�

    }spr_dissolve;
    Microsoft::WRL::ComPtr<ID3D11Buffer> dissolve_constant_buffer;



};

