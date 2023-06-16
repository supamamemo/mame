#include "gltf_model.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "../tinygltf-release/tiny_gltf.h"

#include "misc.h"
#include <stack>
#include "shader.h"
#include "texture.h"


gltf_model::gltf_model(ID3D11Device* device, const std::string& filename) : filename(filename)
{
    tinygltf::Model gltf_model;

    tinygltf::TinyGLTF tiny_gltf;
    std::string error, warning;
    bool succeeded{ false };
    if (filename.find(".glb") != std::string::npos)
    {
        succeeded = tiny_gltf.LoadBinaryFromFile(&gltf_model, &error, &warning, filename.c_str());
    }
    else if (filename.find(".gltf") != std::string::npos)
    {
        succeeded = tiny_gltf.LoadASCIIFromFile(&gltf_model, &error, &warning, filename.c_str());
    }

    _ASSERT_EXPR_A(warning.empty(), warning.c_str());
    _ASSERT_EXPR_A(error.empty(), warning.c_str());
    _ASSERT_EXPR_A(succeeded, L"Failed to load glTF file");
    for (std::vector<tinygltf::Scene>::const_reference gltf_scene : gltf_model.scenes)
    {
        scene& scene{ scenes.emplace_back() };
        scene.name = gltf_model.scenes.at(0).name;
        scene.nodes = gltf_model.scenes.at(0).nodes;
    }

    fetch_nodes(gltf_model);
    fetch_meshes(device, gltf_model);
    fetch_materials(device, gltf_model);
    fetch_textures(device, gltf_model);

    // TODO: This is a force-brute programming. may cause bugs.
    const std::map<std::string, buffer_view>& vertex_buffer_views{
        meshes.at(0).primitives.at(0).vertex_buffer_views };
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, vertex_buffer_views.at("POSITION").format, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "NORMAL", 0, vertex_buffer_views.at("NORMAL").format, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "TANGENT", 0, vertex_buffer_views.at("TANGENT").format, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "TEXCOORD", 0, vertex_buffer_views.at("TEXCOORD_0").format, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "JOINTS", 0, vertex_buffer_views.at("JOINTS_0").format, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "WEIGHTS", 0, vertex_buffer_views.at("WEIGHTS_0").format, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    create_vs_from_cso(device, "./resources/Shader/gltf_model_vs.cso", vertex_shader.ReleaseAndGetAddressOf(),
        input_layout.ReleaseAndGetAddressOf(), input_element_desc, _countof(input_element_desc));
    create_ps_from_cso(device, "./resources/Shader/gltf_model_ps.cso", pixel_shader.ReleaseAndGetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(primitive_constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HRESULT hr;
    hr = device->CreateBuffer(&buffer_desc, nullptr, primitive_cbuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}


void gltf_model::fetch_nodes(const tinygltf::Model& gltf_model)
{
    for (std::vector<tinygltf::Node>::const_reference gltf_node : gltf_model.nodes)
    {
        node& node{ nodes.emplace_back() };
        node.name = gltf_node.name;
        node.skin = gltf_node.skin;
        node.mesh = gltf_node.mesh;
        node.children = gltf_node.children;
        if (!gltf_node.matrix.empty())
        {
            DirectX::XMFLOAT4X4 matrix;
            for (size_t row = 0; row < 4; row++)
            {
                for (size_t column = 0; column < 4; column++)
                {
                    matrix(row, column) = static_cast<float>(gltf_node.matrix.at(4 * row + column));
                }
            }

            DirectX::XMVECTOR S, T, R;
            bool succeed = DirectX::XMMatrixDecompose(&S, &R, &T, DirectX::XMLoadFloat4x4(&matrix));
            _ASSERT_EXPR(succeed, L"Failed to decompose matrix.");

            DirectX::XMStoreFloat3(&node.scale, S);
            DirectX::XMStoreFloat4(&node.rotation, R);
            DirectX::XMStoreFloat3(&node.translation, T);
        }
        else
        {
            if (gltf_node.scale.size() > 0)
            {
                node.scale.x = static_cast<float>(gltf_node.scale.at(0));
                node.scale.y = static_cast<float>(gltf_node.scale.at(1));
                node.scale.z = static_cast<float>(gltf_node.scale.at(2));
            }
            if (gltf_node.translation.size() > 0)
            {
                node.translation.x = static_cast<float>(gltf_node.translation.at(0));
                node.translation.y = static_cast<float>(gltf_node.translation.at(1));
                node.translation.z = static_cast<float>(gltf_node.translation.at(2));
            }
            if (gltf_node.rotation.size() > 0)
            {
                node.rotation.x = static_cast<float>(gltf_node.rotation.at(0));
                node.rotation.y = static_cast<float>(gltf_node.rotation.at(1));
                node.rotation.z = static_cast<float>(gltf_node.rotation.at(2));
            }
        }
    }
    cumulate_transforms(nodes);
}


void gltf_model::cumulate_transforms(std::vector<node>& nodes)
{
    std::stack<DirectX::XMFLOAT4X4> parent_global_transforms;
    std::function<void(int)> traverse{ [&](int node_index)->void
        {
            node& node{nodes.at(node_index)};
            DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(node.scale.x,node.scale.y,node.scale.z) };
            DirectX::XMMATRIX R{ DirectX::XMMatrixRotationQuaternion(
                DirectX::XMVectorSet(node.rotation.x,node.rotation.y,node.rotation.z,node.rotation.w)) };
            DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(node.translation.x,node.translation.y,node.translation.z) };
            DirectX::XMStoreFloat4x4(&node.global_transform, S * R * T *
                DirectX::XMLoadFloat4x4(&parent_global_transforms.top()));
            for (int child_index : node.children)
            {
                parent_global_transforms.push(node.global_transform);
                traverse(child_index);
                parent_global_transforms.pop();
            }
    } };
    for (std::vector<int>::value_type node_index : scenes.at(0).nodes)
    {
        parent_global_transforms.push({ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
        traverse(node_index);
        parent_global_transforms.pop();
    }
}

gltf_model::buffer_view gltf_model::make_buffer_view(const tinygltf::Accessor& accessor)
{
    buffer_view buffer_view;
    
    switch (accessor.type)
    {
    case TINYGLTF_TYPE_SCALAR:
        switch (accessor.componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            buffer_view.format = DXGI_FORMAT_R16_UINT;
            buffer_view.stride_in_bytes = sizeof(USHORT);
            break;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            buffer_view.format = DXGI_FORMAT_R32_UINT;
            buffer_view.stride_in_bytes = sizeof(UINT);
            break;
        default:
            _ASSERT_EXPR(FALSE, L"This accessor component type is not supported.");
            break;
        }
        break;
    case TINYGLTF_TYPE_VEC2:
        switch (accessor.componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            buffer_view.format = DXGI_FORMAT_R32G32_FLOAT;
            buffer_view.stride_in_bytes = sizeof(FLOAT) * 2;
            break;
        default:
            _ASSERT_EXPR(FALSE, L"This accessor component type is not supported.");
            break;
        }
        break;
    case TINYGLTF_TYPE_VEC3:
        switch (accessor.componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            buffer_view.format = DXGI_FORMAT_R32G32B32_FLOAT;
            buffer_view.stride_in_bytes = sizeof(FLOAT) * 3;
            break;
        default:
            _ASSERT_EXPR(FALSE, L"This accessor component type is not supported.");
            break;
        }
        break;
    case TINYGLTF_TYPE_VEC4:
        switch (accessor.componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            buffer_view.format = DXGI_FORMAT_R16G16B16A16_UINT;
            buffer_view.stride_in_bytes = sizeof(USHORT) * 4;
            break;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            buffer_view.format = DXGI_FORMAT_R32G32B32A32_UINT;
            buffer_view.stride_in_bytes = sizeof(UINT) * 4;
            break;
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            buffer_view.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            buffer_view.stride_in_bytes = sizeof(FLOAT) * 4;
            break;
        default:
            _ASSERT_EXPR(FALSE, L"This accessor component type is not supported.");
            break;
        }
        break;
    default:
        _ASSERT_EXPR(FALSE, L"This accessor type is not supported.");
        break;
    }
    buffer_view.size_in_bytes = static_cast<UINT>(accessor.count * buffer_view.stride_in_bytes);
    return buffer_view;
}

void gltf_model::fetch_meshes(ID3D11Device* device, const tinygltf::Model& gltf_model)
{
    HRESULT hr;
    for (std::vector<tinygltf::Mesh>::const_reference gltf_mesh : gltf_model.meshes)
    {
        mesh& mesh{ meshes.emplace_back() };
        mesh.name = gltf_mesh.name;
        for (std::vector<tinygltf::Primitive>::const_reference gltf_primitive : gltf_mesh.primitives)
        {
            mesh::primitive& primitive{ mesh.primitives.emplace_back() };
            primitive.material = gltf_primitive.material;

            // Create index buffer
            const tinygltf::Accessor& gltf_accessor{ gltf_model.accessors.at(gltf_primitive.indices) };
            const tinygltf::BufferView& gltf_buffer_view{ gltf_model.bufferViews.at(gltf_accessor.bufferView) };

            primitive.index_buffer_view = make_buffer_view(gltf_accessor);

            D3D11_BUFFER_DESC buffer_desc{};
            buffer_desc.ByteWidth = static_cast<UINT>(primitive.index_buffer_view.size_in_bytes);
            buffer_desc.Usage = D3D11_USAGE_DEFAULT;
            buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            D3D11_SUBRESOURCE_DATA subresource_data{};
            subresource_data.pSysMem = gltf_model.buffers.at(gltf_buffer_view.buffer).data.data()
                + gltf_buffer_view.byteOffset + gltf_accessor.byteOffset;
            hr = device->CreateBuffer(&buffer_desc, &subresource_data,
                primitive.index_buffer_view.buffer.ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

            // Create vertex buffers
            for (std::map<std::string, int>::const_reference gltf_attribute : gltf_primitive.attributes)
            {
                const tinygltf::Accessor& gltf_accessor{ gltf_model.accessors.at(gltf_attribute.second) };
                const tinygltf::BufferView& gltf_buffer_view{ gltf_model.bufferViews.at(gltf_accessor.bufferView) };

                buffer_view vertex_buffer_view{ make_buffer_view(gltf_accessor) };

                D3D11_BUFFER_DESC buffer_desc{};
                buffer_desc.ByteWidth = static_cast<UINT>(vertex_buffer_view.size_in_bytes);
                buffer_desc.Usage = D3D11_USAGE_DEFAULT;
                buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                D3D11_SUBRESOURCE_DATA subresource_data{};
                subresource_data.pSysMem = gltf_model.buffers.at(gltf_buffer_view.buffer).data.data()
                    + gltf_buffer_view.byteOffset + gltf_accessor.byteOffset;
                hr = device->CreateBuffer(&buffer_desc, &subresource_data,
                    vertex_buffer_view.buffer.ReleaseAndGetAddressOf());
                _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

                primitive.vertex_buffer_views.emplace(std::make_pair(gltf_attribute.first, vertex_buffer_view));
            }

            // Add dummy attributes if any are missing.
            const std::unordered_map<std::string, buffer_view> attributes{
                {"TANGENT",{DXGI_FORMAT_R32G32B32A32_FLOAT}},
                {"TEXCOORD_0",{DXGI_FORMAT_R32G32_FLOAT}},
                {"JOINTS_0",{DXGI_FORMAT_R16G16B16A16_UINT}},
                {"WEIGHTS_0",{DXGI_FORMAT_R32G32B32A32_FLOAT}},
            };
            for (std::unordered_map<std::string, buffer_view>::const_reference attribute : attributes)
            {
                if (primitive.vertex_buffer_views.find(attribute.first) == primitive.vertex_buffer_views.end())
                {
                    primitive.vertex_buffer_views.insert(std::make_pair(attribute.first, attribute.second));
                }
            }
        }
    }
}

void gltf_model::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world)
{
    // material_resource_view�I�u�W�F�N�g�̃o�C���h
    immediate_context->PSSetShaderResources(0, 1, material_resource_view.GetAddressOf());

    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
    immediate_context->IASetInputLayout(input_layout.Get());
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    std::function<void(int)> traverse{ [&](int node_index)->void {
        const node& node{nodes.at(node_index)};
        if (node.mesh > -1)
        {
            const mesh& mesh{ meshes.at(node.mesh) };
            for (std::vector<mesh::primitive>::const_reference primitive : mesh.primitives)
            {
                ID3D11Buffer* vertex_buffers[]{
                    primitive.vertex_buffer_views.at("POSITION").buffer.Get(),
                    primitive.vertex_buffer_views.at("NORMAL").buffer.Get(),
                    primitive.vertex_buffer_views.at("TANGENT").buffer.Get(),
                    primitive.vertex_buffer_views.at("TEXCOORD_0").buffer.Get(),
                    primitive.vertex_buffer_views.at("JOINTS_0").buffer.Get(),
                    primitive.vertex_buffer_views.at("WEIGHTS_0").buffer.Get(),
                };
                UINT strides[]{
                    static_cast<UINT>(primitive.vertex_buffer_views.at("POSITION").stride_in_bytes),
                    static_cast<UINT>(primitive.vertex_buffer_views.at("NORMAL").stride_in_bytes),
                    static_cast<UINT>(primitive.vertex_buffer_views.at("TANGENT").stride_in_bytes),
                    static_cast<UINT>(primitive.vertex_buffer_views.at("TEXCOORD_0").stride_in_bytes),
                    static_cast<UINT>(primitive.vertex_buffer_views.at("JOINTS_0").stride_in_bytes),
                    static_cast<UINT>(primitive.vertex_buffer_views.at("WEIGHTS_0").stride_in_bytes),
                };
                UINT offsets[_countof(vertex_buffers)]{ 0 };
                immediate_context->IASetVertexBuffers(0, _countof(vertex_buffers), vertex_buffers, strides, offsets);
                immediate_context->IASetIndexBuffer(primitive.index_buffer_view.buffer.Get(),
                    primitive.index_buffer_view.format, 0);

                primitive_constants primitive_data{};
                primitive_data.material = primitive.material;
                primitive_data.has_tangent = primitive.vertex_buffer_views.at("TANGENT").buffer != NULL;
                primitive_data.skin = node.skin;
                DirectX::XMStoreFloat4x4(&primitive_data.world,
                    DirectX::XMLoadFloat4x4(&node.global_transform) * DirectX::XMLoadFloat4x4(&world));
                immediate_context->UpdateSubresource(primitive_cbuffer.Get(), 0, 0, &primitive_data, 0, 0);
                immediate_context->VSSetConstantBuffers(0, 1, primitive_cbuffer.GetAddressOf());
                immediate_context->PSSetConstantBuffers(0, 1, primitive_cbuffer.GetAddressOf());

                // texture set
                const material& material{ materials.at(primitive.material) };
                const int texture_indices[]
                {
                    material.data.pbr_metallic_roughness.basecolor_texture.index,
                    material.data.pbr_metallic_roughness.metallic_roughness_texture.index,
                    material.data.normal_texture.index,
                    material.data.emissive_texture.index,
                    material.data.occlusion_texture.index,
                };
                ID3D11ShaderResourceView* null_shader_resource_view{};
                std::vector<ID3D11ShaderResourceView*> shader_resource_views(_countof(texture_indices));
                for (int texture_index = 0; texture_index < shader_resource_views.size(); ++texture_index)
                {
                    shader_resource_views.at(texture_index) = texture_indices[texture_index] > -1 ?
                        texture_resource_views.at(textures.at(texture_indices[texture_index]).source).Get() :
                        null_shader_resource_view;
                }
                immediate_context->PSSetShaderResources(1, static_cast<UINT>(shader_resource_views.size()),
                    shader_resource_views.data());
                immediate_context->DrawIndexed(static_cast<UINT>(primitive.index_buffer_view.count()), 0, 0);
            }
        }
        for (std::vector<int>::value_type child_index : node.children)
        {
            traverse(child_index);
        }
    } };
    for (std::vector<int>::value_type node_index : scenes.at(0).nodes)
    {
        traverse(node_index);
    }
}

void gltf_model::fetch_materials(ID3D11Device* device, const tinygltf::Model& gltf_model)
{
    for (std::vector<tinygltf::Material>::const_reference gltf_material : gltf_model.materials)
    {
        std::vector<material>::reference material = materials.emplace_back();

        material.name = gltf_material.name;

        material.data.emissive_factor[0] = static_cast<float>(gltf_material.emissiveFactor.at(0));
        material.data.emissive_factor[1] = static_cast<float>(gltf_material.emissiveFactor.at(1));
        material.data.emissive_factor[2] = static_cast<float>(gltf_material.emissiveFactor.at(2));

        material.data.alpha_mode = gltf_material.alphaMode == "OPAQUE" ?
            0 : gltf_material.alphaMode == "MASK" ? 1 : gltf_material.alphaMode == "BLEND" ? 2 : 0;
        material.data.alpha_cutoff = static_cast<float>(gltf_material.alphaCutoff);
        material.data.double_sided = gltf_material.doubleSided ? 1 : 0;

        material.data.pbr_metallic_roughness.basecolor_factor[0] =
            static_cast<float>(gltf_material.pbrMetallicRoughness.baseColorFactor.at(0));
        material.data.pbr_metallic_roughness.basecolor_factor[1] =
            static_cast<float>(gltf_material.pbrMetallicRoughness.baseColorFactor.at(1));
        material.data.pbr_metallic_roughness.basecolor_factor[2] =
            static_cast<float>(gltf_material.pbrMetallicRoughness.baseColorFactor.at(2));
        material.data.pbr_metallic_roughness.basecolor_factor[3] =
            static_cast<float>(gltf_material.pbrMetallicRoughness.baseColorFactor.at(3));
        material.data.pbr_metallic_roughness.basecolor_texture.index =
            gltf_material.pbrMetallicRoughness.baseColorTexture.index;
        material.data.pbr_metallic_roughness.basecolor_texture.texcoord =
            gltf_material.pbrMetallicRoughness.baseColorTexture.texCoord;
        material.data.pbr_metallic_roughness.metallic_factor =
            static_cast<float>(gltf_material.pbrMetallicRoughness.roughnessFactor);
        material.data.pbr_metallic_roughness.metallic_roughness_texture.index =
            gltf_material.pbrMetallicRoughness.metallicRoughnessTexture.index;
        material.data.pbr_metallic_roughness.metallic_roughness_texture.texcoord;

        material.data.normal_texture.index = gltf_material.normalTexture.index;
        material.data.normal_texture.texcoord = gltf_material.occlusionTexture.texCoord;
        material.data.normal_texture.scale = static_cast<float>(gltf_material.normalTexture.scale);

        material.data.occlusion_texture.index = gltf_material.occlusionTexture.index;
        material.data.occlusion_texture.texcoord = gltf_material.occlusionTexture.texCoord;
        material.data.occlusion_texture.strength =
            static_cast<float>(gltf_material.occlusionTexture.strength);

        material.data.emissive_texture.index = gltf_material.emissiveTexture.index;
        material.data.emissive_texture.texcoord = gltf_material.emissiveTexture.texCoord;
    }

    // Create material data as shader resource view on GPU
    std::vector<material::cbuffer> material_data;
    for (std::vector<material>::const_reference material : materials)
    {
        material_data.emplace_back(material.data);
    }

    HRESULT hr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> material_buffer;
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(material::cbuffer) * material_data.size());
    buffer_desc.StructureByteStride = sizeof(material::cbuffer);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = material_data.data();
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, material_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = DXGI_FORMAT_UNKNOWN;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    shader_resource_view_desc.Buffer.NumElements = static_cast<UINT>(material_data.size());
    hr = device->CreateShaderResourceView(material_buffer.Get(),
        &shader_resource_view_desc, material_resource_view.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void gltf_model::fetch_textures(ID3D11Device* device, const tinygltf::Model& gltf_model)
{
    HRESULT hr{ S_OK };
    for (const tinygltf::Texture& gltf_texture : gltf_model.textures)
    {
        texture& texture{ textures.emplace_back() };
        texture.name = gltf_texture.name;
        texture.source = gltf_texture.source;
    }
    for (const tinygltf::Image& gltf_image : gltf_model.images)
    {
        image& image{ images.emplace_back() };
        image.name = gltf_image.name;
        image.width = gltf_image.width;
        image.height = gltf_image.height;
        image.component = gltf_image.component;
        image.bits = gltf_image.bits;
        image.pixel_type = gltf_image.pixel_type;
        image.buffer_view = gltf_image.bufferView;
        image.mime_type = gltf_image.mimeType;
        image.url = gltf_image.uri;
        image.as_is = gltf_image.as_is;

        if (gltf_image.bufferView > -1)
        {
            const tinygltf::BufferView& buffer_view{ gltf_model.bufferViews.at(gltf_image.bufferView) };
            const tinygltf::Buffer& buffer{ gltf_model.buffers.at(buffer_view.buffer) };
            const byte* data = buffer.data.data() + buffer_view.byteOffset;

            ID3D11ShaderResourceView* texture_resource_view{};
            hr = load_texture_from_memory(device, data, buffer_view.byteLength, &texture_resource_view);
            if (hr == S_OK)
            {
                texture_resource_views.emplace_back().Attach(texture_resource_view);
            }
        }
        else
        {
            const std::filesystem::path path(filename);
            ID3D11ShaderResourceView* shader_resource_view{};
            D3D11_TEXTURE2D_DESC texture2d_desc;
            std::wstring filename{
                path.parent_path().concat(L"/").wstring() +
                std::wstring(gltf_image.uri.begin(),gltf_image.uri.end()) };
            hr = load_texture_from_file(device, filename.c_str(), &shader_resource_view, &texture2d_desc);
            if (hr == S_OK)
            {
                texture_resource_views.emplace_back().Attach(shader_resource_view);
            }
        }
    }
}
