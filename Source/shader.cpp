#include "shader.h"
#include "Mame\Graphics\Camera.h"
#include <fstream>

#include "misc.h"

#include "skinned_mesh.h"


HRESULT create_vs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    if (input_layout)
    {
        hr = device->CreateInputLayout(input_element_desc, num_elements,
            cso_data.get(), cso_sz, input_layout);
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
    
    return hr;
}

HRESULT create_ps_from_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return hr;
}

Shader::Shader(ID3D11Device* device)
{
    // �萔�o�b�t�@
    {
        HRESULT hr{ S_OK };

        D3D11_BUFFER_DESC buffer_desc{};
        buffer_desc.ByteWidth = sizeof(CbScene);
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;
        hr = device->CreateBuffer(&buffer_desc, nullptr, sceneConstantBuffer[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        buffer_desc.ByteWidth = sizeof(CBParametric);
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;
        hr = device->CreateBuffer(&buffer_desc, nullptr, sceneConstantBuffer[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // �u�����h�X�e�[�g
    {
        HRESULT hr{ S_OK };

        D3D11_BLEND_DESC blend_desc{};
        // �s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��ɁA
        //�}���`�T���v�����O��@�Ƃ��ăA���t�@���J�o���b�W���g�p���邩�ǂ������w�肵�܂�
        blend_desc.AlphaToCoverageEnable = FALSE;
        // ���������_�[�^�[�Q�b�g�œƗ������u�����h��L���ɂ��邩�ǂ�����ݒ肵�܂��B
        // �Ɨ������u�����h��L���ɂ���ɂ́ATURE�ɐݒ肵�܂�
        // FALSE�ɐݒ肷��ƁARenderTarget[0]�����o�[�݂̂��g�p����܂��BRenderTarget[1..7]�͖�������܂�
        blend_desc.IndependentBlendEnable = FALSE;
        // D3D11_RENDER_TARGET_BLEND_DESC[8]
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, blendState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // �[�x�X�e���V���X�e�[�g
    {
        HRESULT hr{ S_OK };

        D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
        // 0 �[�x�e�X�g�F�I���@�[�x���C�g�F�I��
        depth_stencil_desc.DepthEnable = TRUE;	// �[�x�e�X�g��L���ɂ��܂�
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// �[�x�f�[�^�ŕύX�ł���[�x�X�e���V���o�b�t�@�[�̈ꕔ����肵�܂�
        depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // �[�x�f�[�^�Ɗ����f�[�^���r����֐�
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 1 �[�x�e�X�g�F�I���@�[�x���C�g�F�I�t
        depth_stencil_desc.DepthEnable = TRUE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 2 �[�x�e�X�g�F�I�t�@�[�x���C�g�F�I��
        depth_stencil_desc.DepthEnable = FALSE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[2].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 3 �[�x�e�X�g�F�I�t�@�[�x���C�g�F�I�t
        depth_stencil_desc.DepthEnable = FALSE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[3].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // ���X�^���C�U�[�X�e�[�g
    {
        HRESULT hr{ S_OK };

        D3D11_RASTERIZER_DESC rasterizer_desc{};
        rasterizer_desc.FillMode = D3D11_FILL_SOLID;    // �����_�����O���Ɏg�p����t�B�����[�h
        rasterizer_desc.CullMode = D3D11_CULL_BACK;     // �w�肵�������ɖʂ���O�p�`���`�悳��Ȃ����Ƃ�����
        rasterizer_desc.FrontCounterClockwise = TRUE;  // �O�p�`���\�����������肷��
        rasterizer_desc.DepthBias = 0;                  // �w�肳�ꂽ�s�N�Z���ɒǉ����ꂽ�[�x�l�B
        rasterizer_desc.DepthBiasClamp = 0;             // �s�N�Z���̍ő�[�x�o�C�A�X
        rasterizer_desc.SlopeScaledDepthBias = 0;       // �w�肳�ꂽ�s�N�Z���̌X���̃X�J���[
        rasterizer_desc.DepthClipEnable = TRUE;         // �����Ɋ�Â��ăN���b�s���O��L���ɂ��܂�
        rasterizer_desc.ScissorEnable = FALSE;          // �n�T�~�l�p�`�J�����O��L���ɂ��܂��B
        rasterizer_desc.MultisampleEnable = FALSE;      // �}���`�T���v�����O�A���`�G�C���A�V���O(MSAA)�����_�[�^�[�Q�b�g��
                                                        // �l�p�`�܂��̓A���t�@���̃A���`�G�C���A�V���O�A���S���Y�����g�p���邩�ǂ������w�肵�܂�
        rasterizer_desc.AntialiasedLineEnable = FALSE;  // ����A���`�G�C���A�V���O��L���ɂ��邩�ǂ���

        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        rasterizer_desc.CullMode = D3D11_CULL_NONE;
        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[3].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
        rasterizer_desc.CullMode = D3D11_CULL_BACK;
        rasterizer_desc.AntialiasedLineEnable = TRUE;
        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
        rasterizer_desc.CullMode = D3D11_CULL_NONE;
        rasterizer_desc.AntialiasedLineEnable = TRUE;
        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[2].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // �T���v���X�e�[�g
    {
        HRESULT hr{ S_OK };

        D3D11_SAMPLER_DESC sampler_desc{};
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
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
        hr = device->CreateSamplerState(&sampler_desc, samplerState[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        hr = device->CreateSamplerState(&sampler_desc, samplerState[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        hr = device->CreateSamplerState(&sampler_desc, samplerState[2].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
}

void Shader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc)
{
    Camera& camera = Camera::Instance();

    // �T���v���[�X�e�[�g
    dc->PSSetSamplers(0, 1, samplerState[0].GetAddressOf());
    dc->PSSetSamplers(1, 1, samplerState[1].GetAddressOf());
    dc->PSSetSamplers(2, 1, samplerState[2].GetAddressOf());

    dc->OMSetDepthStencilState(depthStencilState[0].Get(), 1);

    dc->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

    dc->RSSetState(rasterizerState[0].Get());

    // �r���[�v���W�F�N�V�����ϊ��s����v�Z���A�����萔�o�b�t�@�ɃZ�b�g����
#if 0
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    dc->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    DirectX::XMMATRIX P{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,100.0f) };
    //DirectX::XMMATRIX P{ DirectX::XMMatrixOrthographicLH(view.viewWitdh,view.viewHeight,view.nearZ,view.farZ) };

    DirectX::XMVECTOR eye{ DirectX::XMVectorSet(0.0f,0.0f,-10.0f,1.0f) };
    DirectX::XMVECTOR focus{ DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f) };
    DirectX::XMMATRIX V{ DirectX::XMMatrixLookAtLH(eye,focus,up) };
#else
    camera.SetPerspectiveFov(dc);
#endif
    

    CbScene data{};
    //DirectX::XMStoreFloat4x4(&data.viewProjection, V * P);
    DirectX::XMStoreFloat4x4(&data.viewProjection, camera.GetV() * camera.GetP());
    data.lightDirection = { view.position.x,view.position.y,view.position.z,view.position.w };
    data.camera_position = { view.camera.x,view.camera.y,view.camera.z,view.camera.w };
    dc->UpdateSubresource(sceneConstantBuffer[0].Get(), 0, 0, &data, 0, 0);
    dc->VSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());
    dc->PSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());

#ifdef USE_IMGUI
    //DrawDebug();
#endif
}

void Shader::SetState(ID3D11DeviceContext* dc, int RastarizeState, int DepthStencilState, int SamplerState)
{
    dc->PSSetSamplers(SamplerState, 1, samplerState[SamplerState].GetAddressOf());
    dc->RSSetState(rasterizerState[RastarizeState].Get());
    dc->OMSetDepthStencilState(depthStencilState[DepthStencilState].Get(), 1);
}

void Shader::Draw(ID3D11DeviceContext* dc, Model* model)
{
}

void Shader::End(ID3D11DeviceContext* dc)
{
}

void Shader::DrawDebug()
{
    Camera& camera = Camera::Instance();

    ImGui::Begin("view");

    if (ImGui::TreeNode("camera"))
    {
        ImGui::SliderFloat("wight", &view.viewWitdh,1.0f,10000.0f);
        ImGui::SliderFloat("height", &view.viewHeight, 1.0f, 10000.0f);
        ImGui::SliderFloat("nearZ", &view.nearZ, 1.0f, 10000.0f);
        ImGui::SliderFloat("farZ", &view.farZ, 1.0f, 10000.0f);

        ImGui::DragFloat4("positon", &view.position.x);
        ImGui::DragFloat4("camera", &view.camera.x);

        ImGui::TreePop();
    }

    
    camera.DrawDebug();

    ImGui::End();
}
