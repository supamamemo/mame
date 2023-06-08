#pragma once
#include <WICTextureLoader.h>

// �e�N�X�`���ǂݍ���
HRESULT load_texture_from_file(ID3D11Device* device, const wchar_t* filename,
    ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc);

void release_all_textures();

// �_�~�[�e�N�X�`���쐬�֐�
HRESULT make_dummy_texture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view,
    DWORD value/*0xAABBGGRR*/, UINT dimension);

// load_texture_from_memory
HRESULT load_texture_from_memory(ID3D11Device* device, const void* data, size_t size,
    ID3D11ShaderResourceView** shader_resource_view);