#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include <wrl.h>

class sprite_batch
{
public:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;  // ���_�V�F�[�_�[�C���^�[�t�F�C�X
    // ���_�V�F�[�_�[�C���^�[�t�F�C�X�́A���_�V�F�[�_�[�X�e�[�W�𐧌䂷��
    // ���s�\�v���O����(���_�V�F�[�_�[)���Ǘ����܂��B
    // <���_�V�F�[�_�[> ���_�f�[�^���󂯎���ăX�N���[�����W�ɕϊ�����v�Z������
    // <���_�f�[�^> ���W�A�F�Ȃǂ̃|���S�����\������ׂɕK�v�ȏ��

    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;    // �s�N�Z���V�F�[�_�[�C���^�[�t�F�C�X
    Microsoft::WRL::ComPtr<ID3D11PixelShader> c_pixel_shader;    // �s�N�Z���V�F�[�_�[�C���^�[�t�F�C�X
    // �s�N�Z���V�F�[�_�[�C���^�[�t�F�C�X�́A�s�N�Z���V�F�[�_�[�X�e�[�W�𐧌䂷��
    // ���s�\�v���O����(�s�N�Z���V�F�[�_�[)���Ǘ����܂�
    // <�s�N�Z���V�F�[�_�[> �|���S���̐F���s�N�Z���P�ʂŌv�Z����

    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;    // ���̓��C�A�E�g�C���^�[�t�F�C�X
    // ���̓��C�A�E�g�C���^�[�t�F�C�X�́A���������Ƀ��C�A�E�g����Ă��钸�_�f�[�^��
    // �O���t�B�b�N�X�p�C�v���C���̓��̓A�Z���u���[�X�e�[�W�Ƀt�B�[�h������@�̒�`��ێ����܂�
    // <���̓��C�A�E�g> ���_�f�[�^�ɂǂ̂悤�ȏ�񂪂���̂���`���A�V�F�[�_�[�ɋ�������

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;        // �o�b�t�@�[�C���^�[�t�F�C�X
    // �o�b�t�@�[�C���^�[�t�F�C�X�́A��\�����������ł���o�b�t�@�[���\�[�X�ɃA�N�Z�X���܂�
    // �o�b�t�@�[�͒ʏ�A���_�f�[�^�܂��̓C���f�b�N�X�f�[�^���i�[���܂�
    // <���_�o�b�t�@> ���_�f�[�^��GPU�ň����ׂ̊i�[�p�o�b�t�@

    // <�`��X�e�[�g> �|���S���̕\�����@��A���s��r���@�Ȃǂ̗l�X�Ȑݒ������
    //      ����̓��X�^���C�U�X�e�[�g��ݒ肷��

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view[2]; // �V�F�[�_�[���\�[�X�r���[�C���^�[�t�F�C�X
    D3D11_TEXTURE2D_DESC texture2d_desc;

    // ���_�t�H�[�}�b�g
    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcord;
    };

    const size_t max_vertices;
    std::vector<vertex>vertices;

public:
    void render(ID3D11DeviceContext* immediate_context);
    void render(ID3D11DeviceContext* immediate_context,
        float dx, float dy, // �Z�`�̍���̍��W(�X�N���[�����W�n)
        float dw, float dh, // �Z�`�̃T�C�Y(�X�N���[�����W�n)
        float r, float g, float b, float a, // �F
        float angle/*degree*/);
    void render(ID3D11DeviceContext* immediate_context,
        float dx, float dy, float dw, float dh,
        float r, float g, float b, float a,
        float angle/*degree*/,
        float sx, float sy, float sw, float sh);

    void render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh);

    sprite_batch(ID3D11Device* device, const wchar_t* filename, size_t max_sprites);
    ~sprite_batch();

    void begin(ID3D11DeviceContext* immediate_context);
    void end(ID3D11DeviceContext* immediate_context);
    void change(ID3D11DeviceContext* immediate_context);

    inline auto rotate(float& x, float& y, float cx, float cy, float angle)
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
};
