#include "framework.h"

#pragma comment(lib, "d3d11.lib")

#include "./Mame/Scene/SceneTitle.h"
#include "./Mame/Scene/SceneManager.h"

framework::framework(HWND hwnd)
    : hwnd(hwnd),
    graphics(hwnd),
    input(hwnd)
{
}

bool framework::initialize()
{
    HRESULT hr{ S_OK };
    
    framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), 1280, 720);
    bit_block_transfer = std::make_unique<fullscreen_quad>(graphics.GetDevice());
    framebuffers[1] = std::make_unique<framebuffer>(graphics.GetDevice(), 1280 / 2, 720 / 2);
    create_ps_from_cso(graphics.GetDevice(), "luminance_extraction_ps.cso", pixel_shaders[0].GetAddressOf());
    create_ps_from_cso(graphics.GetDevice(), "blur_ps.cso", pixel_shaders[1].GetAddressOf());


    // シーンタイトル
    Mame::Scene::SceneManager::Instance().ChangeScene(new SceneTitle);

    return true;
}


void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
#ifdef USE_IMGUI
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
#endif


    // 入力更新処理
    input.Update();

    // シーン更新処理
    Mame::Scene::SceneManager::Instance().Update(elapsed_time);

    //ImGui::ShowDemoWindow();

#ifdef USE_IMGUI

#endif
}

void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
    Graphics& graphics = Graphics::Instance();
    //ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
    //ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
    //ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();



#if 0
#if 0
    ID3D11RenderTargetView* null_render_target_views[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
    immediate_context->OMSetRenderTargets(_countof(null_render_target_views), null_render_target_views, 0);
    ID3D11ShaderResourceView* null_shader_resource_views[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{};
    immediate_context->VSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);
    immediate_context->PSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);
#endif
    HRESULT hr{ S_OK };

#if 0
    FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
    immediate_context->ClearRenderTargetView(render_target_view, color);
    immediate_context->ClearDepthStencilView(depth_stencil_view,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
#endif

    //サンプラーステートオブジェクトをバインドする
    //immediate_context->PSSetSamplers(0, 1, sampler_states[0].GetAddressOf());
    //immediate_context->PSSetSamplers(1, 1, sampler_states[1].GetAddressOf());
    //immediate_context->PSSetSamplers(2, 1, sampler_states[2].GetAddressOf());

    // sprite::render
#if 0   
    sprites[0]->render(immediate_context, imgui_debug.dx, imgui_debug.dy,
        imgui_debug.dw, imgui_debug.dh,
        imgui_debug.r, imgui_debug.g, imgui_debug.b, imgui_debug.a,
        imgui_debug.angle);
#endif

    //immediate_context->OMSetDepthStencilState(depth_stencil_states[3].Get(), 1);
    // ブレンディングステートオブジェクトを設定する
    //immediate_context->OMSetBlendState(blend_states[0].Get(), nullptr, 0xFFFFFFFF);

    // ラスタライザステートの切り替え
    //immediate_context->RSSetState(rasterizer_states[imgui_debug.rasterizer_state].Get());

    //sprites[0]->render(immediate_context.Get(), 0, 0, 1280, 720, 1, 1, 1, 1, 0,0,0,100,100); // 画像全体を画面全体に描画する
    //sprites[1]->render(immediate_context.Get(), 500, 200, 200, 200, 1, 1, 1, 1, 0, 0, 0, 140, 240);

    float x{ 0 };
    float y{ 0 };
#if 0
    for (size_t i = 0; i < 1092; ++i)
    {
        sprites[1]->render(immediate_context,
            x, static_cast<float>(static_cast<int>(y) % 720), 64, 64,
            1, 1, 1, 1, 0, 140 * 0, 240 * 0, 140, 240);
        x += 32;
        if (x > 1280 - 64)
        {
            x = 0;
            y += 24;
        }
    }
#elif 0
//#else
    sprite_batches[0]->begin(immediate_context.Get());
    for (size_t i = 0; i < 1092; ++i)
    {
        sprite_batches[0]->render(immediate_context.Get(),
            x, static_cast<float>(static_cast<int>(y) % 720), 64, 64,
            1, 1, 1, 1, 0, 140 * 0, 240 * 0, 140, 240);
        x += 32;
        if (x > 1280 - 64)
        {
            x = 0;
            y += 24;
        }

        if (i > 1092 / 2)
        {
            //sprite_batches[0]->change(immediate_context.Get());
        }
    }
    sprite_batches[0]->end(immediate_context.Get());
#endif


    //  2d背景画像
#if 0
    framebuffers[0]->clear(immediate_context);
    framebuffers[0]->activate(immediate_context);
    immediate_context->OMSetDepthStencilState(depth_stencil_states[3].Get(), 1);
    immediate_context->RSSetState(rasterizer_states[3].Get());
    sprite_batches[0]->begin(immediate_context);
    sprite_batches[0]->render(immediate_context,
        0, 0, 1280, 720,
        imgui_debug.color.x, imgui_debug.color.y, imgui_debug.color.z, imgui_debug.color.w,
        imgui_debug.angle);
    sprite_batches[0]->end(immediate_context);
#endif

    // ラスタライザステートの切り替え
    //immediate_context->RSSetState(rasterizer_states[imgui_debug.rasterizer_state].Get());

    // ビュー・プロジェクション変換行列を計算し、それを定数バッファにセットする
#if 0
    D3D11_VIEWPORT viewport;
    UINT unm_viewports{ 1 };
    immediate_context->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    DirectX::XMMATRIX P{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,100.0f) };

    DirectX::XMVECTOR eye{ DirectX::XMVectorSet(imgui_debug.eye.x,imgui_debug.eye.y,imgui_debug.eye.z,imgui_debug.eye.w) };
    DirectX::XMVECTOR focus{ DirectX::XMVectorSet(imgui_debug.focus.x,imgui_debug.focus.y,imgui_debug.focus.z,imgui_debug.focus.w) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(imgui_debug.up.x,imgui_debug.up.y,imgui_debug.up.z,imgui_debug.up.w) };
    DirectX::XMMATRIX V{ DirectX::XMMatrixLookAtLH(eye,focus,up) };

    scene_constants data{};
    DirectX::XMStoreFloat4x4(&data.view_projection, V * P);
    data.light_direction = { imgui_debug.right.x,imgui_debug.right.y,imgui_debug.right.z,imgui_debug.right.w };
    data.camera_position = { imgui_debug.camera.x,imgui_debug.camera.y,imgui_debug.camera.z,imgui_debug.camera.w };
    immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &data, 0, 0);
    immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());
    immediate_context->PSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());
    
    immediate_context->UpdateSubresource(constant_buffers[1].Get(), 0, 0, &imgui_debug.parametric_constants, 0, 0);
    immediate_context->PSSetConstantBuffers(2, 1, constant_buffers[1].GetAddressOf());

#endif

    // 座標系の変換
    //const DirectX::XMFLOAT4X4 coordinate_system_transforms[]
    //{
    //    { -1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 },    // 0:RHS Y-UP
    //    { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 },     // 1:LHS Y-UP
    //    { -1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1 },   // 2:RHS Z-UP
    //    { 1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1 },     // 3:LHS Z-UP
    //};
    // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
    // 単位をセンチメートルからメートルに変更するには、「scale_factor」を 0.01 に設定します
    //const float scale_factor = 1.0f;
    //const float scale_factor = 0.01f;
    //DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0])
    //    * DirectX::XMMatrixScaling(scale_factor,scale_factor,scale_factor) };

    // 拡大縮小(S)・回転(R)・平行移動(T)
    //DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(imgui_debug.geoScale.x,imgui_debug.geoScale.y,imgui_debug.geoScale.z) };
    //DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(imgui_debug.geoAngle.x,imgui_debug.geoAngle.y,imgui_debug.geoAngle.z) };
    //DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(imgui_debug.geoPosition.x,imgui_debug.geoPosition.y,imgui_debug.geoPosition.z) };

    //DirectX::XMFLOAT4X4 world;
    //DirectX::XMStoreFloat4x4(&world, C * S * R * T);


    //immediate_context->OMSetDepthStencilState(depth_stencil_states[0].Get(), 1);
#if 0
    geometric_primitives[0]->render(immediate_context.Get(), world,
        { imgui_debug.color.x,imgui_debug.color.y,imgui_debug.color.z,imgui_debug.color.w });

    //static_meshes[0]->render(immediate_context.Get(), world,
    //    { imgui_debug.color.x,imgui_debug.color.y,imgui_debug.color.z,imgui_debug.color.w });
#endif
#if 0
    int clip_index{ 0 };
    int frame_index{ 0 };
    static float animation_tick{ 0 };

    animation& animation{ skinned_meshes[0]->animation_clips.at(clip_index) };
    frame_index = static_cast<int>(animation_tick * animation.sampling_rate);
    if (frame_index > animation.sequence.size() - 1)
    {
        frame_index = 0;
        animation_tick = 0;
    }
    else
    {
        animation_tick += elapsed_time;
    }
    animation::keyframe& keyframe{ animation.sequence.at(frame_index) };
#else
    //animation::keyframe keyframe;
    //const animation::keyframe* keyframes[2]{
    //    &skinned_meshes[0]->animation_clips.at(0).sequence.at(imgui_debug.modelBlending[0]),
    //    &skinned_meshes[0]->animation_clips.at(0).sequence.at(imgui_debug.modelBlending[1])
    //};
    //skinned_meshes[0]->blend_animations(keyframes, 0.5f, keyframe);
    //skinned_meshes[0]->update_animation(keyframe);
#endif

#if 0
    DirectX::XMStoreFloat4(&keyframe.nodes.at(30).rotation,
        DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), imgui_debug.modelAngle));
    keyframe.nodes.at(30).translation.x = imgui_debug.modelLenght;
    skinned_meshes[0]->update_animation(keyframe);
#endif
    //skinned_meshes[0]->render(immediate_context.Get(), world,
    //    { imgui_debug.color.x,imgui_debug.color.y,imgui_debug.color.z,imgui_debug.color.w },
    //    & keyframe);

   // framebuffers[0]->deactivate(immediate_context);

    // オフスクリーンレンダリングtest
#if 0
    bit_block_transfer->blit(immediate_context.Get(),
        framebuffers[0]->shader_resource_views[0].GetAddressOf(), 0, 1);
#endif
    //immediate_context->RSSetState(rasterizer_states[3].Get());
    //immediate_context->OMSetDepthStencilState(depth_stencil_states[3].Get(), 1);
    //framebuffers[1]->clear(immediate_context);
    //framebuffers[1]->activate(immediate_context);
    //bit_block_transfer->blit(immediate_context,
    //    framebuffers[0]->shader_resource_views[0].GetAddressOf(), 0, 1, pixel_shaders[0].Get());
    //framebuffers[1]->deactivate(immediate_context);
    // 高輝度成分test
#if 0
    bit_block_transfer->blit(immediate_context.Get(),
        framebuffers[1]->shader_resource_views[0].GetAddressOf(), 0, 1);
#endif
    //ID3D11ShaderResourceView* shader_resource_views[2]
    //{ framebuffers[0]->shader_resource_views[0].Get(),framebuffers[1]->shader_resource_views[0].Get() };
    //bit_block_transfer->blit(immediate_context, shader_resource_views, 0, 2, pixel_shaders[1].Get());

    // gltf_model描画
    //immediate_context->RSSetState(rasterizer_states[3].Get());
    //immediate_context->OMSetDepthStencilState(depth_stencil_states[0].Get(), 1);
    //gltf_models[0]->render(immediate_context, world);
#endif

 
    // todo scenemanager
    Mame::Scene::SceneManager::Instance().Render(elapsed_time);



#ifdef USE_IMGUI
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

    UINT sync_interval{ 0 };
    graphics.GetSwapChain()->Present(sync_interval, 0); 
}

bool framework::uninitialize()
{
    // シーン終了化
    Mame::Scene::SceneManager::Instance().Clear();

    return false;
}

framework::~framework()
{

}


