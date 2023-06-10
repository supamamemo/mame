#pragma once
#include "BaseScene.h"

#include "../../sprite.h"
#include "../../sprite_batch.h"

#include "../../Game/Player.h"
#include "../../Game/sprite_dissolve.h"

class SceneTitle : public Mame::Scene::BaseScene
{
public:
    SceneTitle() {}
    ~SceneTitle()override {}

    void Initialize()override;      // ������
    void Finalize()override;        // �I����
    void Begin()override;           // ���t���[����ԍŏ��ɌĂ΂��
    void Update(float elapesdTime)override;          // �X�V����
    void End()override;             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime)override;          // �`�揈��
    void DrawDebug()override;

public:
    // test
    std::unique_ptr<sprite> dummy_sprite;
    std::unique_ptr<sprite> dissolve_sprite;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> sprite_vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> sprite_input_layout;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> sprite_pixel_shader;

    

    Microsoft::WRL::ComPtr<ID3D11SamplerState>  samplerState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

public:
    Player* player = nullptr;

    Sprite_dissolve sprite_dissolve;

    int RS = 3;
    int DS = 0;
    int SS = 0;


    struct Spr
    {
        DirectX::XMFLOAT2 pos{};
        DirectX::XMFLOAT2 posD{ 100,100 };
    }spr[2] = {
        {{200.0f,420.0f},{300.0f,300.0f}},
        {{600.0f,420.0f},{300.0f,300.0f}} };

    // UVScroll
    struct scroll_constants
    {
        DirectX::XMFLOAT2 scroll_direction;
        DirectX::XMFLOAT2 scroll_dummy;
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> scroll_constant_buffer;
    DirectX::XMFLOAT2 scroll_direction{};
    DirectX::XMFLOAT2 scroll_value{ 0.001f,0.0f };

    // mask
    D3D11_TEXTURE2D_DESC mask_texture2dDesc;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mask_texture[5];
    int mask_texture_value = 0;
    struct dissolve_constants
    {
        DirectX::XMFLOAT4 parameters;   // x:�f�B�]���u�K���ʁA
                                        // y:
                                        // z: ����臒l
                                        // w:��
        DirectX::XMFLOAT4 edgeColor;    // ���̐F
    };
    float dissolve_value{ 0.0f };
    float dissolve_value1{ 0.0f };
    Microsoft::WRL::ComPtr<ID3D11Buffer> dissolve_constant_buffer;

    // mask edge
    float edge_threshold{ 0.2f };   // ����臒l
    DirectX::XMFLOAT4 edgeColor{ 1,0,0,1 };    // ���̐F


};

