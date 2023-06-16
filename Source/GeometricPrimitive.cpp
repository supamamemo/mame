#include "GeometricPrimitive.h"
#include "shader.h"
#include "misc.h"



// �R���X�g���N�^
GeometricPrimitive::GeometricPrimitive(ID3D11Device* device)
{
    // ����
#if 0
    vertex vertices[24]
    {
        {{0.0f,0.0f,0.0f},{0,0,-1}}, // 0
        {{1.0f,-1.0f,0.0f},{0,0,-1}}, // 1
        {{-1.0f,-1.0f,0.0f},{0,0,-1}}, // 2
        {{-1.0f,0.0f,0.0f},{0,0,-1}}, // 2
        
        
    };

    uint32_t indices[36]
    {
        0,1,2,
        2,3,0,
    };


    create_com_buffers(device, vertices, 24, indices, 36);
#endif


    // �����`
#if 1
    DirectX::XMFLOAT3 position[]
    {
        { -1.0f, -1.0f, -1.0f },    // 0
        { 1.0f, -1.0f, -1.0f },     // 1
        { 1.0f, 1.0f, -1.0f },      // 2
        { -1.0f, 1.0f, -1.0f },     // 3

        { -1.0f, -1.0f, 1.0f },     // 4

        { 1.0f, -1.0f, 1.0f },      // 5
        { 1.0f, 1.0f, 1.0f },       // 6
        
        { -1.0f, 1.0f, 1.0f },      // 7
    };

    vertex vertices[24]
    {
#if 1
        // front
        { position[0], { 0, 0, -1 } },  // 0        
        { position[1], { 0, 0, -1 } },  // 1
        { position[2], { 0, 0, -1 } },  // 2
        { position[3], { 0, 0, -1 } },  // 3

        // right
        { position[1], { 1, 0, 0 } },   // 1
        { position[5], { 1, 0, 0 } },   // 5
        { position[6], { 1, 0, 0 } },   // 6
        { position[2], { 1, 0, 0 } },   // 2

        // back
        { position[5], { 0, 0, 1 } },   // 5
        { position[4], { 0, 0, 1 } },   // 4
        { position[7], { 0, 0, 1} },    // 7
        { position[6], { 0, 0, 1} },    // 6

        // left        
        { position[4], { -1, 0, 0 } },  // 4
        { position[0], { -1, 0, 0 } },  // 0
        { position[3], { -1, 0, 0 } },  // 3
        { position[7], { -1, 0, 0} },   // 7

        // top
        { position[3], { 0, 1, 0 } },   // 3
        { position[2], { 0, 1, 0 } },   // 2
        { position[6], { 0, 1, 0 } },   // 6
        { position[7], { 0, 1, 0 } },   // 7

        // bottom
        { position[4], { 0, -1, 0 } },  // 4
        { position[5], { 0, -1, 0 } },  // 5
        { position[1], { 0, -1, 0 } },  // 1
        { position[0], { 0, -1, 0 } },  // 0
#endif

    };
    // �T�C�Y��1.0�̐������̃f�[�^���쐬����(�d�S�����_�ɂ���)�B�������̂̃R���g���[���|�C���g����8�A
    // 1�̃R���g���[���|�C���g�̈ʒu�ɂ͖@���̌������Ⴄ���_��3����̂Œ��_���̑�����8x3=24��
    // ���_���z��(vertices)�ɂ��ׂĒ��_�̈ʒu�E�@������i�[����B

    uint32_t indices[36]
    {
#if 1
        // front
        0,3,2,
        0,2,1,
        // right
        4,7,6,
        4,6,5,
        // back
        8,11,10, 
        8,10,9,
        // left
        12,15,14, 
        12,14,13,
        // top
        16,19,18, 
        16,18,17,
        //bottom
        20,23,22,
        20,22,21
#endif
    };
    // �������̂�6�ʎ����A1�̖ʂ�2�̎O�p�`�|���S���ō\�������̂ŎO�p�`�|���S���̑�����6x2=12�A
    // �������̂�`�悷�邽�߂�12��̎O�p�`�|���S���`�悪�K�v�A����ĎQ�Ƃ���钸�_����12x3=36��A
    // �O�p�`�|���S�����Q�Ƃ��钸�_���̃C���f�b�N�X�i���_�ԍ��j��b���m�揇�ɔz��iindices�j�Ɋi�[����B
    // ���v��肪�\�ʂɂȂ�悤�Ɋi�[���邱�ƁB

    // �����`
    create_com_buffers(device, vertices, 24, indices, 36);
#endif

    HRESULT hr{ S_OK };

    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},

    };
    create_vs_from_cso(device, "geometric_primitive_vs.cso", vertex_shader.GetAddressOf(),
        input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    create_ps_from_cso(device, "geometric_primitive_ps.cso", pixel_shader.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

GeometricPrimitive::GeometricPrimitive(ID3D11Device* device, DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 range)
{

    DirectX::XMFLOAT3 position[]
    {
        { center.x - range.x, center.y, center.z - range.z },
        { center.x + range.x, center.y, center.z - range.z },
        { center.x + range.x, center.y + range.y * 2, center.z - range.z },
        { center.x - range.x, center.y + range.y * 2, center.z - range.z },

        { center.x - range.x, center.y, center.z + range.z },

        { center.x + range.x, center.y, center.z + range.z },
        { center.x + range.x, center.y + range.y * 2, center.z + range.z },

        { center.x - range.x, center.y + range.y * 2, center.z + range.z },


        //{ -1.0f, -1.0f, -1.0f },    // 0
        //{ 1.0f, -1.0f, -1.0f },     // 1
        //{ 1.0f, 1.0f, -1.0f },      // 2
        //{ -1.0f, 1.0f, -1.0f },     // 3

        //{ -1.0f, -1.0f, 1.0f },     // 4

        //{ 1.0f, -1.0f, 1.0f },      // 5
        //{ 1.0f, 1.0f, 1.0f },       // 6

        //{ -1.0f, 1.0f, 1.0f },      // 7
    };

    vertex vertices[24]
    {

        // front
        { position[0], { 0, 0, -1 } },  // 0        
        { position[1], { 0, 0, -1 } },  // 1
        { position[2], { 0, 0, -1 } },  // 2
        { position[3], { 0, 0, -1 } },  // 3

        // right
        { position[1], { 1, 0, 0 } },   // 1
        { position[5], { 1, 0, 0 } },   // 5
        { position[6], { 1, 0, 0 } },   // 6
        { position[2], { 1, 0, 0 } },   // 2

        // back
        { position[5], { 0, 0, 1 } },   // 5
        { position[4], { 0, 0, 1 } },   // 4
        { position[7], { 0, 0, 1} },    // 7
        { position[6], { 0, 0, 1} },    // 6

        // left        
        { position[4], { -1, 0, 0 } },  // 4
        { position[0], { -1, 0, 0 } },  // 0
        { position[3], { -1, 0, 0 } },  // 3
        { position[7], { -1, 0, 0} },   // 7

        // top
        { position[3], { 0, 1, 0 } },   // 3
        { position[2], { 0, 1, 0 } },   // 2
        { position[6], { 0, 1, 0 } },   // 6
        { position[7], { 0, 1, 0 } },   // 7

        // bottom
        { position[4], { 0, -1, 0 } },  // 4
        { position[5], { 0, -1, 0 } },  // 5
        { position[1], { 0, -1, 0 } },  // 1
        { position[0], { 0, -1, 0 } },  // 0
    };

    uint32_t indices[36]
    {
        // front
        0,3,2,
        0,2,1,
        // right
        4,7,6,
        4,6,5,
        // back
        8,11,10,
        8,10,9,
        // left
        12,15,14,
        12,14,13,
        // top
        16,19,18,
        16,18,17,
        //bottom
        20,23,22,
        20,22,21
    };

    create_com_buffers(device, vertices, 24, indices, 36);

    HRESULT hr{ S_OK };

    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},

    };
    create_vs_from_cso(device, "geometric_primitive_vs.cso", vertex_shader.GetAddressOf(),
        input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    create_ps_from_cso(device, "geometric_primitive_ps.cso", pixel_shader.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

// render
void GeometricPrimitive::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
    uint32_t stride{ sizeof(vertex) };
    uint32_t offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(input_layout.Get());


    // �V�F�[�_�[�֘A
    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

    constants data{ world,material_color };
    immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
    immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    index_buffer->GetDesc(&buffer_desc);
    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);

        
}


void GeometricPrimitive::create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
{
    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC buffer_desc{};
    D3D11_SUBRESOURCE_DATA subresource_data{};
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    subresource_data.pSysMem = vertices;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresource_data.pSysMem = indices;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    //HRESULT hr{ S_OK };

    //D3D11_BUFFER_DESC buffer_desc{};
    //D3D11_SUBRESOURCE_DATA subresource_data{};
    //buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
    //buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    //buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //buffer_desc.CPUAccessFlags = 0;
    //buffer_desc.MiscFlags = 0;
    //buffer_desc.StructureByteStride = 0;
    //subresource_data.pSysMem = vertices;
    //subresource_data.SysMemPitch = 0;
    //subresource_data.SysMemSlicePitch = 0;
    //hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    //buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    //buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    //buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    //subresource_data.pSysMem = indices;
    //hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}
