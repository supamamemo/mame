#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
#include <vector>

class static_mesh
{
public:
    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 texcoord;
    };
    struct constants
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 material_color;
    };
    struct subset
    {
        std::wstring usemtl;
        uint32_t index_start{ 0 };  // start positionof index buffer
        uint32_t index_count{ 0 };  // number of vertices (indices)
    };
    std::vector<subset> subsets;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;

    struct material
    {
        std::wstring name;
        DirectX::XMFLOAT4 Ka{ 0.2f,0.2f,0.2f,1.0f };
        DirectX::XMFLOAT4 Kd{ 0.8f,0.8f,0.8f,1.0f };
        DirectX::XMFLOAT4 Ks{ 1.0f,1.0f,1.0f,1.0f };
        std::wstring texture_filename[2];
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view[2]; 
    };
    std::vector<material>materials;

public:
    static_mesh(ID3D11Device* device, const wchar_t* obj_filename, bool frontTex);
    virtual ~static_mesh() = default;

    void render(ID3D11DeviceContext* immediate_context,
        const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color);

protected:
    void create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count,
        uint32_t* indices, size_t index_count);
};