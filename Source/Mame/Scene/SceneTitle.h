#pragma once
#include "BaseScene.h"

#include "../../sprite.h"
#include "../../sprite_batch.h"

#include "../../Game/spriteDissolve.h"

#include "../../Game/TitlePlayer.h"
#include "../../Game/TitleEnemyTofu.h"

class SceneTitle : public Mame::Scene::BaseScene
{
public:
    SceneTitle();
    ~SceneTitle()       override {}

    void Initialize()   override;                   // ������
    void Finalize()     override;                   // �I����
    void Begin()        override;                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()          override;                   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug()    override;

public:
    // test
    std::unique_ptr<Sprite> dummy_sprite                = nullptr;
    std::unique_ptr<Sprite> dissolve_sprite             = nullptr;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> sprite_vertex_shader     = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>  sprite_input_layout      = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>  sprite_pixel_shader      = nullptr;

    Microsoft::WRL::ComPtr<ID3D11SamplerState>      samplerState        = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   rasterizerState     = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState   = nullptr;
    Microsoft::WRL::ComPtr<ID3D11BlendState>        blendState          = nullptr;

public:
    std::unique_ptr<TitlePlayer> titlePlayer_ = nullptr;    // �^�C�g���p�v���C���[

    static const int ENEMY_TOFU_COUNT = 3;  // �G�l�~�[�����ő吔
    std::unique_ptr<TitleEnemyTofu> titleEnemyTofu_[ENEMY_TOFU_COUNT] = {}; //�@�^�C�g���p�G�l�~�[����


    // fadeOut/fadeIn�p�̃f�B�]���u
    std::unique_ptr<SpriteDissolve> spriteDissolve;

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


    // title model
    std::unique_ptr<Model> castleModel = nullptr;
    std::unique_ptr<Model> groundModel = nullptr;

};

