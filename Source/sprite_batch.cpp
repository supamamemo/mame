#include "sprite_batch.h"
#include "misc.h"
#include <sstream>

#include "../External/DirectXTK-main/Inc/WICTextureLoader.h"

#include "texture.h"
#include "shader.h"

void sprite_batch::render(ID3D11DeviceContext* immediate_context)
{
    // <1> ���_�o�b�t�@�[�̃o�C���h
    UINT stride{ sizeof(vertex) };
    UINT offset{ 0 };
    // <IASetVertexBuffers()> ���_�o�b�t�@�[��CPU�L�q�q�n���h����ݒ肵�܂�
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

    // <2> �v���~�e�B�u�^�C�v�y�уf�[�^�̏���Ɋւ�����̃o�C���h
    // <IASetPrimitiveTopology> �v���~�e�B�u�^�Ɋւ�����ƁA
    // ���̓A�Z���u���[�X�e�[�W�̓��̓f�[�^���L�q���L�q����f�[�^�������o�C���h���܂�
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // <3> ���̓��C�A�E�g�I�u�W�F�N�g�̃o�C���h
    // <IASetInputLayout> ���̓��C�A�E�g�I�u�W�F�N�g����̓A�Z���u���[�X�e�[�W���o�C���h���܂�
    immediate_context->IASetInputLayout(input_layout.Get());

    // <4> �V�F�[�_�[�̃o�C���h
    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

    // <5> �v���~�e�B�u�̕`��
    immediate_context->Draw(4, 0);
}

#if 1
void sprite_batch::render(ID3D11DeviceContext* immediate_context,
    float dx, float dy, float dw, float dh,
    float r, float g, float b, float a,
    float angle
)
{

    // <1> �X�N���[���i�r���[�|�[�g�j�̃T�C�Y���擾����
#if 1
    // D3D11_VIEWPORT �r���[�|�[�g�̐��@���`���܂�
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    // <RSGetViewport> ���X�^���C�U�[�X�e�[�W�Ƀo�C���h���ꂽ�r���[�|�[�g�̔z����擾���܂�
    immediate_context->RSGetViewports(&num_viewports, &viewport);
#endif
    // <2> render�����o�֐��̈���(dx,dy,dw,dh)����Z�`�̊p���_�̈ʒu�i�X�N���[�����W�n�j���v�Z����
#if 1
    // (x0, y0) *----* (x1, y1)
    //          |   /|
    //          |  / |
    //          | /  |
    //          |/   |
    // (x2, y2) *----* (x3, y3)

    // left-top
    float x0{ dx };
    float y0{ dy };
    // right-top
    float x1{ dx + dw };
    float y1{ dy };
    // left-bottom
    float x2{ dx };
    float y2{ dy + dh };
    // right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };
#endif

    // ��`��]
#if 1
    auto rotate = [](float& x, float& y, float cx, float cy, float angle)
    {
        x -= cx;
        y -= cy;

        float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
        float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
        float tx{ x }, ty{ y };
        x = cos * tx + -sin * ty;
        y = sin * tx + cos * ty;

        x += cx;
        y += cy;
    };
    // ��]�̒��S��Z�`�̒��S�_�ɂ����ꍇ
    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;
    rotate(x0, y0, cx, cy, angle);
    rotate(x1, y1, cx, cy, angle);
    rotate(x2, y2, cx, cy, angle);
    rotate(x3, y3, cx, cy, angle);
#endif

    // <3> �X�N���[�����W�n����NDC�ւ̍��W�ϊ��������Ȃ�
#if 1
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;
#endif
    // <4> �v�Z���ʂŒ��_�o�b�t�@�I�u�W�F�N�g���X�V����
#if 1
    HRESULT hr{ S_OK };
    // D3D11_MAPPED_SUBRESOURCE�\���� �T�u���\�[�X�f�[�^�ւ̃A�N�Z�X��񋟂��܂�
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0, 0 };
        vertices[1].position = { x1, y1, 0 };
        vertices[2].position = { x2, y2, 0 };
        vertices[3].position = { x3, y3, 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r,g,b,a };

        vertices[0].texcord = { 0, 0 };
        vertices[1].texcord = { 1, 0 };
        vertices[2].texcord = { 0, 1 };
        vertices[3].texcord = { 1, 1 };
    }

    immediate_context->Unmap(vertex_buffer.Get(), 0);
#endif    
    // �V�F�[�_�[���\�[�X�̃o�C���h
    immediate_context->PSSetShaderResources(0, 1, shader_resource_view[0].GetAddressOf());

    // render����
    render(immediate_context);
}
#else
void sprite_batch::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle)
{
    //�X�N���[��(�r���[�|�[�g)�̃T�C�Y���擾����
#if 1
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);
#endif
    //render�����o�֐��̈���(dx, dy, dw, dh)����Z�a�̊e���_�ʒu(�X�N���[�����W�n)���v�Z
#if 1
  // (x0, y0) ---- (x1, y1)
  //          |   /|
  //          |  / |
  //          | /  |
  //          |/   |
  // (x2, y2) ---- (x3, y3)

  //left-top
    float x0{ dx };
    float y0{ dy };
    //right-top
    float x1{ dx + dw };
    float y1{ dy };
    //left-bottom
    float x2{ dx };  float y2{ dy + dh };
    //right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };
#endif

    auto rotate = [](float& x, float& y, float cx, float cy, float angle)
    {
        x -= cx;
        y -= cy;

        float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
        float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
        float tx{ x }, ty{ y };
        x = cos * tx + -sin * ty;
        y = sin * tx + cos * ty;

        x += cx;
        y += cy;
    };

    //��]�̒��S��Z�`�̒��S�_�Ƃ����ꍇ#if 1
#if 1
    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;
    rotate(x0, y0, cx, cy, angle);
    rotate(x1, y1, cx, cy, angle);
    rotate(x2, y2, cx, cy, angle);
    rotate(x3, y3, cx, cy, angle);
#endif

    //�X�N���[�����W����NDC�ւ̍��W�ϊ��������Ȃ�
#if 1
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0 * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;
#endif
    //�v�Z���ʂŒ��_�o�b�t�@�I�u�W�F�N�g���X�V����
#if 1
    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0, 0 };
        vertices[1].position = { x1, y1, 0 };
        vertices[2].position = { x2, y2, 0 };
        vertices[3].position = { x3, y3, 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r, g, b, a };

        vertices[0].texcord = { 0, 0 };
        vertices[1].texcord = { 1, 0 };
        vertices[2].texcord = { 0, 1 };
        vertices[3].texcord = { 1, 1 };
    }

    immediate_context->Unmap(vertex_buffer.Get(), 0);
#endif

    //�V�F�[�_�[���\�[�X�̃o�C���h
#if 1
    immediate_context->PSSetShaderResources(0, 1, shader_resource_view[0].GetAddressOf());
#endif

    //���_�o�b�t�@�\�̃o�C���h
#if 1
    UINT stride{ sizeof(vertex) };
    UINT offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
    immediate_context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
#endif

    //�v���~�e�B�u�^�C�v����уf�[�^�̏����Ɋւ�����̃o�C���h
#if 1
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
#endif

    //���̓��C�A�E�g�I�u�W�F�N�g�̃o�C���h
#if 1
    immediate_context->IASetInputLayout(input_layout.Get());
#endif

    //�V�F�[�_�[�̃o�C���h
#if 1
    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
#endif

    //�v���~�e�B�u�̕`��
#if 1
    immediate_context->Draw(4, 0);
#endif
}

#endif

// 06 render
void sprite_batch::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle, float sx, float sy, float sw, float sh)
{

    // <1> �X�N���[���i�r���[�|�[�g�j�̃T�C�Y���擾����
#if 1
    // D3D11_VIEWPORT �r���[�|�[�g�̐��@���`���܂�
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    // <RSGetViewport> ���X�^���C�U�[�X�e�[�W�Ƀo�C���h���ꂽ�r���[�|�[�g�̔z����擾���܂�
    immediate_context->RSGetViewports(&num_viewports, &viewport);
#endif
    // <2> render�����o�֐��̈���(dx,dy,dw,dh)����Z�`�̊p���_�̈ʒu�i�X�N���[�����W�n�j���v�Z����
#if 1
    // (x0, y0) *----* (x1, y1)
    //          |   /|
    //          |  / |
    //          | /  |
    //          |/   |
    // (x2, y2) *----* (x3, y3)

    // left-top
    float x0{ dx };
    float y0{ dy };
    // right-top
    float x1{ dx + dw };
    float y1{ dy };
    // left-bottom
    float x2{ dx };
    float y2{ dy + dh };
    // right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };
#endif

    // ��`��]
#if 1



    // ��]�̒��S��Z�`�̒��S�_�ɂ����ꍇ
    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;
    rotate(x0, y0, cx, cy, angle);
    rotate(x1, y1, cx, cy, angle);
    rotate(x2, y2, cx, cy, angle);
    rotate(x3, y3, cx, cy, angle);
#endif

    // <3> �X�N���[�����W�n����NDC�ւ̍��W�ϊ��������Ȃ�
#if 1
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;
#endif

    float u0{ sx / texture2d_desc.Width };
    float v0{ sy / texture2d_desc.Height };
    float u1{ (sx + sw) / texture2d_desc.Width };
    float v1{ (sy + sh) / texture2d_desc.Height };

    vertices.push_back({ { x0, y0, 0 }, { r, g, b, a }, { u0, v0 } });
    vertices.push_back({ { x1, y1, 0 }, { r, g, b, a }, { u1, v0 } });
    vertices.push_back({ { x2, y2, 0 }, { r, g, b, a }, { u0, v1 } });
    vertices.push_back({ { x2, y2, 0 }, { r, g, b, a }, { u0, v1 } });
    vertices.push_back({ { x1, y1, 0 }, { r, g, b, a }, { u1, v0 } });
    vertices.push_back({ { x3, y3, 0 }, { r, g, b, a }, { u1, v1 } });

#if 0
    // <4> �v�Z���ʂŒ��_�o�b�t�@�I�u�W�F�N�g���X�V����
#if 1
    HRESULT hr{ S_OK };
    // D3D11_MAPPED_SUBRESOURCE�\���� �T�u���\�[�X�f�[�^�ւ̃A�N�Z�X��񋟂��܂�
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0, 0 };
        vertices[1].position = { x1, y1, 0 };
        vertices[2].position = { x2, y2, 0 };
        vertices[3].position = { x3, y3, 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r,g,b,a };

        vertices[0].texcord = { sx / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[1].texcord = { (sx + sw) / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[2].texcord = { sx / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
        vertices[3].texcord = { (sx + sw) / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
    }

    immediate_context->Unmap(vertex_buffer, 0);
#endif    
    // �V�F�[�_�[���\�[�X�̃o�C���h
    immediate_context->PSSetShaderResources(0, 1, &shader_resource_view);

    // render����
    render(immediate_context);
#endif
}

void sprite_batch::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh)
{
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);
#if 1
    // (x0, y0) *----* (x1, y1)
    //          |   /|
    //          |  / |
    //          | /  |
    //          |/   |
    // (x2, y2) *----* (x3, y3)

    // left-top
    float x0{ dx };
    float y0{ dy };
    // right-top
    float x1{ dx + dw };
    float y1{ dy };
    // left-bottom
    float x2{ dx };
    float y2{ dy + dh };
    // right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };
#endif
    // <3> �X�N���[�����W�n����NDC�ւ̍��W�ϊ��������Ȃ�
#if 1
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;
#endif
    // <4> �v�Z���ʂŒ��_�o�b�t�@�I�u�W�F�N�g���X�V����
#if 1
    HRESULT hr{ S_OK };
    // D3D11_MAPPED_SUBRESOURCE�\���� �T�u���\�[�X�f�[�^�ւ̃A�N�Z�X��񋟂��܂�
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0, 0 };
        vertices[1].position = { x1, y1, 0 };
        vertices[2].position = { x2, y2, 0 };
        vertices[3].position = { x3, y3, 0 };
    }

    immediate_context->Unmap(vertex_buffer.Get(), 0);
#endif    
    // �V�F�[�_�[���\�[�X�̃o�C���h
    immediate_context->PSSetShaderResources(0, 1, shader_resource_view[0].GetAddressOf());

    // render����
    render(immediate_context);
}

// �R���X�g���N�^
sprite_batch::sprite_batch(ID3D11Device* device, const wchar_t* filename, size_t max_sprites)
    :max_vertices(max_sprites * 6)
{
    // <1> ���_���̃Z�b�g
#if 0
    vertex vertices[]
    {
        { { -0.5, +0.5, 0}, { 1, 1, 1, 1}, { 0, 0} },
        { { +0.5, +0.5, 0}, { 1, 0, 0, 1}, { 1, 0} },
        { { -0.5, -0.5, 0}, { 0, 1, 0, 1}, { 0, 1} },
        { { +0.5, -0.5, 0}, { 0, 0, 1, 1}, { 1, 1} },
    };
#endif
    std::unique_ptr<vertex[]>vertices{ std::make_unique<vertex[]>(max_vertices) };

    // <2> ���_�o�b�t�@�I�u�W�F�N�g�̐���
#if 1
    // D3D11_BUFFER_DESC�\���� 
    D3D11_BUFFER_DESC buffer_desc{};
    //buffer_desc.ByteWidth = sizeof(vertices); // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
    buffer_desc.ByteWidth = sizeof(vertex) * static_cast<UINT>(max_vertices);
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC; // �o�b�t�@�[�̓ǂݎ��Ə������ݕ��@����肵�܂�
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �o�b�t�@�[���p�C�v���C���Ƀo�C���h������@����肷��
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU�A�N�Z�X�t���O�܂���CPU�A�N�Z�X���K�v�Ȃ��ꍇ�͂O
    buffer_desc.MiscFlags = 0; // ���̑��̃t���O�܂��͖��g�p�̏ꍇ�͂O
    buffer_desc.StructureByteStride = 0; // �o�b�t�@�[���\�����o�b�t�@�[��\���ꍇ��
                                    // �o�b�t�@�[�\�����̊e�v�f�̃T�C�Y
    // D3D11_SUBRESOURCE_DATA�\���� �T�u���\�[�X�����������邽�߂̃f�[�^���w�肵�܂�
    D3D11_SUBRESOURCE_DATA subresource_data{};
    //subresource_data.pSysMem = vertices; // �������f�[�^�ւ̃|�C���^�[ 
    subresource_data.pSysMem = vertices.get();
    subresource_data.SysMemPitch = 0;   // �e�N�X�`���̂P�s�̐擪���玟�̍s�܂ł̋���(�o�C�g�P��)
    subresource_data.SysMemSlicePitch = 0; // 1�̐[�x���x���̐擪���玟�̐[�����x���܂ł̋���(�o�C�g�P��)

    /// ? ///
    HRESULT hr{ S_OK };
    /// ? ///

    // <CreateBuffer> 
    // �o�b�t�@�[�i���_�o�b�t�@�[�A�C���f�b�N�X�o�b�t�@�[�A�܂��̓V�F�[�_�[�萔�o�b�t�@�[�j���쐬���܂�
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
#endif

#if 0
    // <3> ���_�V�F�[�_�[�I�u�W�F�N�g�̐���
#if 1
    const char* cso_name{ "sprite_vs.cso" };

    FILE* fp{};
    // �t�@�C�����J�� 
    // ����(�J���ꂽ�t�@�C���ւ̃|�C���^���󂯎��t�@�C���|�C���^�[�ւ̃|�C���^�A
    //      �t�@�C�����A�A�N�Z�X���̎��);
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    // <fseek()> �t�@�C���|�C���^�̈ʒu���w�肵���ʒu�Ɉړ�������
    // <ftell()> �t�@�C���̓ǂݏ����ʒu���擾����
    fseek(fp, 0, SEEK_END);     // <SEEK_END> �t�@�C���̏I���
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);     // <SEEK_SET> �t�@�C���̂͂���

    std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    // <fread()> �t�@�C��fp����size�o�C�g�̃f�[�^��n�ǂݍ���buf�Ɋi�[����
    // ����(�ǂݍ��݃f�[�^�i�[��̃|�C���^,�ǂݍ��݃f�[�^�̃o�C�g�̒���,�ǂݍ��݃f�[�^�̐�,FILE�|�C���^);
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp); // �t�@�C�������

    //HRESULT hr{ S_OK };
    // <CreateVertexShader> �R���p�C�����ꂽ�V�F�[�_���璸�_�V�F�[�_�[�I�u�W�F�N�g���쐬���܂�
    hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, &vertex_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
#endif
#endif

    // <4> ���̓��C�A�E�g�I�u�W�F�N�g�̐���
#if 1
    // D3D11_INPUT_ELEMENT_DESC�\����
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    
#if 0
    // <CreateInputLayout> ���̓A�Z���u���[�X�e�[�W�̓��̓o�b�t�@�[�f�[�^���L�q������̓��C�A�E�g�I�u�W�F�N�g���쐬���܂�
    hr = device->CreateInputLayout(input_element_desc, _countof(input_element_desc),
        cso_data.get(), cso_sz, &input_layout);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
#endif
#endif

#if 0
    // <5> �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̐���
#if 1
    const char* cso_name_p{ "sprite_ps.cso" };

    fp = {};
    fopen_s(&fp, cso_name_p, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    cso_sz = { ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    cso_data = { std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);

    // <CreatePixelShader> �s�N�Z���V�F�[�_�[���쐬���܂�
    hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, &pixel_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
#endif
#endif

    // �V�F�[�_�[�֘A
    create_vs_from_cso(device, "sprite_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
    create_ps_from_cso(device, "sprite_ps.cso", pixel_shader.GetAddressOf());
    create_ps_from_cso(device, "sprite_change_ps.cso", c_pixel_shader.GetAddressOf());

    // �e�N�X�`���̃��[�h
    load_texture_from_file(device, filename, shader_resource_view[0].GetAddressOf(), &texture2d_desc);
    load_texture_from_file(device, L"./resources/cyberpunk.jpg", shader_resource_view[1].GetAddressOf(), &texture2d_desc);

    // �g��Ȃ��Ȃ���
#if 0
    ID3D11Resource* resource{};
    hr = DirectX::CreateWICTextureFromFile(device, filename, &resource, &shader_resource_view);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    resource->Release();

    // �e�N�X�`�����(D3D11_TEXTURE2D_DESC)�̎擾
    ID3D11Texture2D* texture2d{};
    hr = resource->QueryInterface<ID3D11Texture2D>(&texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    texture2d->GetDesc(&texture2d_desc);
    texture2d->Release();
#endif
}

// �f�X�g���N�^
sprite_batch::~sprite_batch()
{
    //shader_resource_view->Release();
}

void sprite_batch::begin(ID3D11DeviceContext* immediate_context)
{
    vertices.clear();
    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
    immediate_context->PSSetShaderResources(0, 1, shader_resource_view[0].GetAddressOf());
}

void sprite_batch::end(ID3D11DeviceContext* immediate_context)
{
    // ���X�����Ă����
#if 0
    // <4> �v�Z���ʂŒ��_�o�b�t�@�I�u�W�F�N�g���X�V����
#if 1
    HRESULT hr{ S_OK };
    // D3D11_MAPPED_SUBRESOURCE�\���� �T�u���\�[�X�f�[�^�ւ̃A�N�Z�X��񋟂��܂�
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0, 0 };
        vertices[1].position = { x1, y1, 0 };
        vertices[2].position = { x2, y2, 0 };
        vertices[3].position = { x3, y3, 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r,g,b,a };

        vertices[0].texcord = { sx / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[1].texcord = { (sx + sw) / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[2].texcord = { sx / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
        vertices[3].texcord = { (sx + sw) / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
    }

    immediate_context->Unmap(vertex_buffer, 0);
#endif    
    // �V�F�[�_�[���\�[�X�̃o�C���h
    immediate_context->PSSetShaderResources(0, 1, &shader_resource_view);

    // <1> ���_�o�b�t�@�[�̃o�C���h
    UINT stride{ sizeof(vertex) };
    UINT offset{ 0 };
    // <IASetVertexBuffers()> ���_�o�b�t�@�[��CPU�L�q�q�n���h����ݒ肵�܂�
    immediate_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

    // <2> �v���~�e�B�u�^�C�v�y�уf�[�^�̏���Ɋւ�����̃o�C���h
    // <IASetPrimitiveTopology> �v���~�e�B�u�^�Ɋւ�����ƁA
    // ���̓A�Z���u���[�X�e�[�W�̓��̓f�[�^���L�q���L�q����f�[�^�������o�C���h���܂�
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // <3> ���̓��C�A�E�g�I�u�W�F�N�g�̃o�C���h
    // <IASetInputLayout> ���̓��C�A�E�g�I�u�W�F�N�g����̓A�Z���u���[�X�e�[�W���o�C���h���܂�
    immediate_context->IASetInputLayout(input_layout);

    // <4> �V�F�[�_�[�̃o�C���h
    immediate_context->VSSetShader(vertex_shader, nullptr, 0);
    immediate_context->PSSetShader(pixel_shader, nullptr, 0);

    // <5> �v���~�e�B�u�̕`��
    immediate_context->Draw(4, 0);
#endif

    // �ύX��

    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    size_t vertex_count = vertices.size();
    _ASSERT_EXPR(max_vertices >= vertex_count, "Buffer overflow");
    vertex* data{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (data != nullptr)
    {
        const vertex* p = vertices.data();
        memcpy_s(data, max_vertices * sizeof(vertex), p, vertex_count * sizeof(vertex));
    }
    immediate_context->Unmap(vertex_buffer.Get(), 0);

    UINT stride{ sizeof(vertex) };
    UINT offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(input_layout.Get());

    immediate_context->Draw(static_cast<UINT>(vertex_count), 0);
}

void sprite_batch::change(ID3D11DeviceContext* immediate_context)
{
    end(immediate_context);
    begin(immediate_context);
    //immediate_context->PSSetShader(c_pixel_shader.Get(), nullptr, 0);
    immediate_context->PSSetShaderResources(0, 1, shader_resource_view[1].GetAddressOf());
}
