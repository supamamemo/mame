#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "../Mame.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../AudioManager.h"

#include "../../misc.h"
#include "../../texture.h"


// todo: 後で消す
#define FADE 1
#define MAME 1

// コンストラクタ
SceneTitle::SceneTitle()
{
    Graphics& graphics = Graphics::Instance();

    // シーンの属性を設定
    SetSceneType(static_cast<int>(Mame::Scene::TYPE::TITLE));

    spriteDissolve = std::make_unique<SpriteDissolve>();

    // mameo
    spriteLoadMameo = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/mameo_Sheet.png");

    // titlePlayer生成
    titlePlayer_ = std::make_unique<TitlePlayer>();

    // titleEnemyTofu生成
    for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
    {
        titleEnemyTofu = std::make_unique<TitleEnemyTofu>();
    }

    // title model
    {
#if MAME
        Graphics& graphics = Graphics::Instance();
        castleModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/castel.fbx", true);
        groundModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/ground.fbx", true);
#endif
    }
}

// 初期化
void SceneTitle::Initialize()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
 
    // カメラ位置リセット
    shader->Initialize();
    
    Camera& camera = Camera::Instance();
    camera.SetAngle(0.0f);

    // fadeout用
#if FADE
    spriteDissolve->Initialize();
    spriteDissolve->SetFadeInTexture({ 0,0,0}, { 1280,720 }, 0.4f, 2);
#else
    spriteDissolve->Initialize();
    spriteDissolve->SetFadeOutTexture({ 0,0 }, { 1280,720 }, 0.4f, 2);
#endif

    // title model
    {
#if MAME
        castleModel->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 5.45f, 0.0f));
        groundModel->GetTransform()->SetPosition(DirectX::XMFLOAT3(4.0f, -1.0f, -36.0f));
        groundModel->GetTransform()->SetScale(DirectX::XMFLOAT3(0.5f, 1.0f, 0.5f));
        groundModel->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, DirectX::XMConvertToRadians(90), 0, 0));
#endif
    }

    const float characterScale = 3.0f;

    // titlePlayer初期化
    titlePlayer_->SetRotationY(ToRadian(0.0f)); // 初期位置を設定
    titlePlayer_->GetTransform()->SetScale(DirectX::XMFLOAT3(characterScale, characterScale, characterScale));
    titlePlayer_->Initialize();

    // titleEnemyTofu初期化
    const float rotationY = ToRadian(20.0f); // 初期位置(豆腐同士の間隔にも影響)
    for (int i = 0; i < ENEMY_TOFU_COUNT; ++i)
    {
        titleEnemyTofu_[i]->SetRotationY(rotationY * (i + 1)); // 初期位置を設定
        titleEnemyTofu_[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(characterScale, characterScale, characterScale));
        titleEnemyTofu_[i]->Initialize();
    }

    HRESULT hr{ S_OK };

    {
        D3D11_SAMPLER_DESC sampler_desc{};
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.MipLODBias = 0;
        sampler_desc.MaxAnisotropy = 16;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sampler_desc.BorderColor[0] = 0;
        sampler_desc.BorderColor[1] = 0;
        sampler_desc.BorderColor[2] = 0;
        sampler_desc.BorderColor[3] = 0;
        sampler_desc.MinLOD = 0;
        sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
        hr = graphics.GetDevice()->CreateSamplerState(&sampler_desc, samplerState.GetAddressOf());
    }
    // depth_stencil_desc
    // 深度テスト：オフ　深度ライト：オフ
    {
        D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
        depth_stencil_desc.DepthEnable = TRUE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        hr = graphics.GetDevice()->CreateDepthStencilState(&depth_stencil_desc, depthStencilState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
    // rasterizer_desc
    {
        D3D11_RASTERIZER_DESC rasterizer_desc{};
        rasterizer_desc.FillMode = D3D11_FILL_SOLID;
        rasterizer_desc.CullMode = D3D11_CULL_BACK;
        rasterizer_desc.FrontCounterClockwise = FALSE;
        rasterizer_desc.DepthBias = 0;
        rasterizer_desc.DepthBiasClamp = 0;
        rasterizer_desc.SlopeScaledDepthBias = 0;
        rasterizer_desc.DepthClipEnable = TRUE;
        rasterizer_desc.ScissorEnable = FALSE;
        rasterizer_desc.MultisampleEnable = FALSE;
        rasterizer_desc.AntialiasedLineEnable = FALSE;
        hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizerState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
    // blend_desc
    {
        // アルファブレンド
        D3D11_BLEND_DESC blend_desc{};
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = graphics.GetDevice()->CreateBlendState(&blend_desc, blendState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // dummy_sprtite
    {
        {
            D3D11_BUFFER_DESC buffer_desc{};
            buffer_desc.Usage = D3D11_USAGE_DEFAULT;
            buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            buffer_desc.CPUAccessFlags = 0;
            buffer_desc.MiscFlags = 0;
            buffer_desc.StructureByteStride = 0;
            buffer_desc.ByteWidth = sizeof(scroll_constants);
            hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, scroll_constant_buffer.GetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


        }

        D3D11_INPUT_ELEMENT_DESC input_element_desc[]
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
                D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };


        //dummy_sprite = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/ground.png");


        // dummy_sprite
        if (dummy_sprite)
        {
            // シェーダー関連
            //create_vs_from_cso(graphics.GetDevice(), "./resources/Shader/sprite_vs.cso", dummy_sprite->GetVertexShaderAddress(), dummy_sprite->GetInputLayoutAddress(), input_element_desc, _countof(input_element_desc));
            //create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/sprite_ps.cso", dummy_sprite->GetPixelShaderAddress());

            // UVScroll
            //create_vs_from_cso(graphics.GetDevice(), "./resources/Shader/UVScroll_vs.cso", dummy_sprite->GetVertexShaderAddress(), dummy_sprite->GetInputLayoutAddress(), input_element_desc, _countof(input_element_desc));
            //create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/UVScroll_ps.cso", dummy_sprite->GetPixelShaderAddress());
        }
    }

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.PlayBGM(BGM::Title, true); // タイトルBGM再生
}

// 終了化
void SceneTitle::Finalize()
{
    for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
    {
        titleEnemyTofu->Finalize();
    }

    titlePlayer_->Finalize();

    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopAllAudio(); // 全音楽停止
}

// 毎フレーム一番最初に呼ばれる
void SceneTitle::Begin()
{
}

// 更新処理
void SceneTitle::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    AudioManager& audioManager = AudioManager::Instance();

    // camera
#if MAME
    Camera& camera = Camera::Instance();
    camera.UpdateTitle(elapsedTime);
#endif

    spriteDissolve->Update();

    // ボタンを押したらfadeOut始まる
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {      
        spriteDissolve->SetFade(true);

        audioManager.StopSE(SE::Select);
        audioManager.PlaySE(SE::Select, false, true); // 選択SE再生
    }

    // todo:後で消す
#ifdef _DEBUG
    if (gamePad.GetButtonDown() & GamePad::BTN_X)camera.SetDebugCamera();
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)camera.SetDebugCamera1();
#endif // _DEBUG


    // fadeOut
    if (spriteDissolve->IsFade())
    {
#if FADE
        spriteDissolve->FadeIn(elapsedTime);

        // fadeIn最後までできたら
        if (spriteDissolve->FadeInReady(0.0f))
        {
            // シーンを切り替える
            Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

            // fadeIn判定をリセット
            spriteDissolve->SetFade(false);
        }
#else
        spriteDissolve->FadeOut(elapsedTime);

        // fadeOut最後までできたら
        if (spriteDissolve->FadeOutReady(2.0f))
        {
            // シーンを切り替える
            Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

            // fadeOut判定をリセット
            spriteDissolve->SetFade(false);
        }
#endif
    }


    // UVScroll
    {
        if (gamePad.GetButton() & GamePad::BTN_X)
            scroll_direction.x += 0.001f;
        if (gamePad.GetButton() & GamePad::BTN_Y)
            scroll_direction.x -= 0.001f;

        // なぜかUVScroll自動でしてくれないからこれで代わりにやってる
        scroll_direction.x += scroll_value.x;
        scroll_direction.y += scroll_value.y;

        // float値の限界を迎えないように設定
        if (scroll_direction.x > 10.0f)scroll_direction.x -= 10.0f;
        if (scroll_direction.x < -10.0f)scroll_direction.x += 10.0f;
        if (scroll_direction.y > 10.0f)scroll_direction.y -= 10.0f;
        if (scroll_direction.y < -10.0f)scroll_direction.y += 10.0f;
    }

    // titlePlayer更新
    titlePlayer_->Update(elapsedTime);

    // titleEnemyTofu更新
    for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
    {
        titleEnemyTofu->Update(elapsedTime);
    }
}

// 毎フレーム一番最後に呼ばれる
void SceneTitle::End()
{
}

// 描画処理
void SceneTitle::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
    ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

    FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
    immediate_context->ClearRenderTargetView(render_target_view, color);
    immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

    RenderContext rc;
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

    Shader* shader = graphics.GetShader();
    shader->Begin(graphics.GetDeviceContext(), rc, 0);

    // titlePlayer描画
    titlePlayer_->Render(elapsedTime);

    // titleEnemyTofu描画
    for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
    {
        titleEnemyTofu->Render(elapsedTime);
    }

    // スプライト描画
    shader->SetState(graphics.GetDeviceContext(), RS, DS, SS);
    // spriteがだせなかった　
#if 1
    //immediate_context->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);
    //immediate_context->OMSetDepthStencilState(depthStencilState.Get(), 1);
    //immediate_context->RSSetState(rasterizerState.Get());


    if(dummy_sprite)
    {
        immediate_context->IASetInputLayout(dummy_sprite->GetInputLayout());
        immediate_context->VSSetShader(dummy_sprite->GetVertexShader(), nullptr, 0);
        immediate_context->PSSetShader(dummy_sprite->GetPixelShader(), nullptr, 0);
        immediate_context->PSSetSamplers(0, 1, samplerState.GetAddressOf());
       // immediate_context->PSSetShaderResources(1, 1, mask_texture.GetAddressOf());

        //定数バッファの更新
        {
            scroll_constants scroll{};
            scroll.scroll_direction.x = scroll_direction.x;
            scroll.scroll_direction.y = scroll_direction.y;
            immediate_context->UpdateSubresource(scroll_constant_buffer.Get(), 0, 0, &scroll, 0, 0);
            immediate_context->VSSetConstantBuffers(2, 1, scroll_constant_buffer.GetAddressOf());
            immediate_context->PSSetConstantBuffers(2, 1, scroll_constant_buffer.GetAddressOf());
        }

        dummy_sprite->render(immediate_context, spr[0].pos.x, spr[0].pos.y, spr[0].posD.x, spr[0].posD.y);
        //dummy_sprite->render(immediate_context, 0, 720 - 260, 500, 260);
        //dummy_sprite->render(immediate_context, 256, 120, 1280 - 256 * 2, 720 - 120 * 2);
    }

    //sprite_dissolve->Render();



#endif

    // title model
    {
#if MAME
        DirectX::XMMATRIX WORLD = castleModel->GetTransform()->CalcWorldMatrix(0.01f);
        DirectX::XMFLOAT4X4 world = {};
        DirectX::XMStoreFloat4x4(&world, WORLD);

        castleModel->skinned_meshes->render(graphics.GetDeviceContext(), world, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
        WORLD = groundModel->GetTransform()->CalcWorldMatrix(0.01f);
        DirectX::XMStoreFloat4x4(&world, WORLD);
        groundModel->skinned_meshes->render(graphics.GetDeviceContext(), world, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
#endif
    }

    // fadeOut
    {
        spriteDissolve->Render();
    }

    // mameo
    if (spriteDissolve->FadeInReady(0.3f))
    {
        spriteLoadMameo->render(graphics.GetDeviceContext(), 400.0f, 300.0f,
            450.0f, 183.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0, 0, 900.0f, 367.0f);
    }

}

// デバッグ用
void SceneTitle::DrawDebug()
{
#ifdef USE_IMGUI
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    shader->DrawDebug();

    titlePlayer_->DrawDebug();

    for (std::unique_ptr<TitleEnemyTofu>& titleEnemyTofu : titleEnemyTofu_)
    {
        titleEnemyTofu->DrawDebug();
    }

    if (ImGui::TreeNode("State"))
    {

        ImGui::SliderInt("rasterize", &RS, 0, 3);
        ImGui::SliderInt("depthstencil", &DS, 0, 3);
        ImGui::SliderInt("samplerstate", &SS, 0, 2);

        ImGui::TreePop();
    }


    if (ImGui::TreeNode("scroll"))
    {
        ImGui::DragFloat2("pos", &spr[0].pos.x);
        ImGui::DragFloat2("posD", &spr[0].posD.x);

        ImGui::SliderFloat2("scroll_direction", &scroll_direction.x, -10.0f, 10.0f);
        ImGui::SliderFloat2("scroll_value", &scroll_value.x, -0.001f, 0.001f);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("dissolve"))
    {
        ImGui::DragFloat2("pos", &spr[1].pos.x);
        ImGui::DragFloat2("posD", &spr[1].posD.x);


        //dissolve_value = dissolve_value1 - 0.4f;


        ImGui::TreePop();
    }

    titlePlayer_->DrawDebug();

    spriteDissolve->DrawDebug();

    //sprite_dissolve.DrawDebug(L"./resources/ground.png");
    //sprite_dissolve->DrawDebug();

    // title model
    {
#if MAME
        ImGui::Begin("castelModel");
        castleModel->GetTransform()->DrawDebug();
        ImGui::End();
        ImGui::Begin("groundModel");
        groundModel->GetTransform()->DrawDebug();
        ImGui::End();
#endif
    }

#endif
}
