#pragma once
#include "BaseScene.h"

#include "../../sprite.h"
#include "../../sprite_batch.h"

#include "../../Game/Player.h"
#include "../../Game/sprite_dissolve.h"

class SceneTitle : public Mame::Scene::BaseScene
{
public:
    SceneTitle();
    ~SceneTitle()override {}

    void Initialize()override;      // 初期化
    void Finalize()override;        // 終了化
    void Begin()override;           // 毎フレーム一番最初に呼ばれる
    void Update(float elapesdTime)override;          // 更新処理
    void End()override;             // 毎フレーム一番最後に呼ばれる
    void Render(float elapsedTime)override;          // 描画処理
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
    std::unique_ptr<Player>player;

    //std::unique_ptr<Sprite_dissolve> sprite_dissolve;

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


};

