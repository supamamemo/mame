#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "../Mame.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "../../misc.h"
#include "../../texture.h"

SceneTitle::SceneTitle()
{
    // player生成
    player = std::make_unique<Player>();
}

// 初期化
void SceneTitle::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    player->Initialize();


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

            // dissolve_sprite
            //{
            //    buffer_desc.ByteWidth = sizeof(dissolve_constants);
            //    hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, dissolve_constant_buffer.GetAddressOf());
            //    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            //}
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

        //dummy_sprite = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/chip_win.png");
        dummy_sprite = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/ground.png");
        
        //dissolve_sprite = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/chip_win.png");
        //dissolve_sprite = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/ground.png");

        // mask
        //{
        //    load_texture_from_file(graphics.GetDevice(), L"./resources/mask/dissolve_animation.png",
        //        mask_texture[0].GetAddressOf(), &mask_texture2dDesc);
        //    load_texture_from_file(graphics.GetDevice(), L"./resources/mask/images.jpg",
        //        mask_texture[1].GetAddressOf(), &mask_texture2dDesc);
        //    load_texture_from_file(graphics.GetDevice(), L"./resources/mask/dissolve_animation2.png",
        //        mask_texture[2].GetAddressOf(), &mask_texture2dDesc);
        //    load_texture_from_file(graphics.GetDevice(), L"./resources/mask/dissolve_3.png",
        //        mask_texture[3].GetAddressOf(), &mask_texture2dDesc);
        //}

        // dummy_sprite
        {
            // シェーダー関連
            create_vs_from_cso(graphics.GetDevice(), "sprite_vs.cso", dummy_sprite->GetVertexShaderAddress(), dummy_sprite->GetInputLayoutAddress(), input_element_desc, _countof(input_element_desc));
            create_ps_from_cso(graphics.GetDevice(), "sprite_ps.cso", dummy_sprite->GetPixelShaderAddress());

            // UVScroll
            create_vs_from_cso(graphics.GetDevice(), "UVScroll_vs.cso", dummy_sprite->GetVertexShaderAddress(), dummy_sprite->GetInputLayoutAddress(), input_element_desc, _countof(input_element_desc));
            create_ps_from_cso(graphics.GetDevice(), "UVScroll_ps.cso", dummy_sprite->GetPixelShaderAddress());
        }
        // dissolve_sprite
        //{
        //    // シェーダー関連
        //    create_vs_from_cso(graphics.GetDevice(), "sprite_vs.cso", dissolve_sprite->GetVertexShaderAddress(), dissolve_sprite->GetInputLayoutAddress(), input_element_desc, _countof(input_element_desc));
        //    create_ps_from_cso(graphics.GetDevice(), "sprite_ps.cso", dissolve_sprite->GetPixelShaderAddress());

        //    // mask
        //    create_vs_from_cso(graphics.GetDevice(), "sprite_dissolve_vs.cso", dissolve_sprite->GetVertexShaderAddress(), dissolve_sprite->GetInputLayoutAddress(), input_element_desc, ARRAYSIZE(input_element_desc));
        //    create_ps_from_cso(graphics.GetDevice(), "sprite_dissolve_ps.cso", dissolve_sprite->GetPixelShaderAddress());
        //}

        //sprite_dissolve.Initialize(static_cast<int>(Dissolve::Fire));
        sprite_dissolve.Initialize(static_cast<int>(Dissolve::Fade), L"./resources/fade.jpg");
        //sprite_dissolve.Initialize(static_cast<int>(Dissolve::Fade), L"./resources/ground.png");
    }
}

// 終了化
void SceneTitle::Finalize()
{
    player->Finalize();
}

// 毎フレーム一番最初に呼ばれる
void SceneTitle::Begin()
{
}

// 更新処理
void SceneTitle::Update(float elapesdTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();


    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
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

    player->Update();
}



// 毎フレーム一番最後に呼ばれる
void SceneTitle::End()
{
}

// 描画処理
void SceneTitle::Render(float elapsedTime)
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
    shader->Begin(graphics.GetDeviceContext(), rc);

    //shader->DrawDebug();

    // プレイヤー描画
    player->Render(elapsedTime);

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

    sprite_dissolve.Render();

    //if (dissolve_sprite)
    //{
    //    immediate_context->IASetInputLayout(dummy_sprite->GetInputLayout());
    //    immediate_context->VSSetShader(dummy_sprite->GetVertexShader(), nullptr, 0);
    //    immediate_context->PSSetShader(dummy_sprite->GetPixelShader(), nullptr, 0);
    //    immediate_context->PSSetSamplers(0, 1, samplerState.GetAddressOf());
    //    immediate_context->PSSetShaderResources(1, 1, mask_texture[mask_texture_value].GetAddressOf());

    //    // 定数バッファの更新
    //    {
    //        dissolve_constants dissolve{};
    //        dissolve.parameters.x = dissolve_value;
    //        dissolve.parameters.y = dissolve_value1;
    //        dissolve.parameters.z = edge_threshold;
    //        dissolve.edgeColor = edgeColor;
    //        immediate_context->UpdateSubresource(dissolve_constant_buffer.Get(), 0, 0, &dissolve, 0, 0);
    //        immediate_context->VSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
    //        immediate_context->PSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
    //    }

    //    dissolve_sprite->render(immediate_context, spr[1].pos.x, spr[1].pos.y, spr[1].posD.x, spr[1].posD.y);
    //}
#endif

}

// デバッグ用
void SceneTitle::DrawDebug()
{
#ifdef USE_IMGUI
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    shader->DrawDebug();

    player->DrawDebug();

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

        ImGui::SliderFloat("dissolve_value", &dissolve_value, 0.0f, 1.0f);
        ImGui::SliderFloat("dissolve_value1", &dissolve_value1, 0.0f, 2.0f);
        //dissolve_value = dissolve_value1 - 0.4f;

        ImGui::SliderInt("mask_texture", &mask_texture_value, 0, 3);

        ImGui::SliderFloat4("edgeColor", &edgeColor.x, 0.0f, 1.0f);

        ImGui::TreePop();
    }

    player->model->skinned_meshes.Drawdebug();

    //sprite_dissolve.DrawDebug(L"./resources/ground.png");
    sprite_dissolve.DrawDebug(L"./resources/fade.jpg");
#endif
}
