#include "GrassBlock.h"

#include "../Mame/Graphics/Graphics.h"

// �R���X�g���N�^
GrassBlock::GrassBlock()
{
    Graphics& graphics = Graphics::Instance();

    // model����
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
}

// �f�X�g���N�^
GrassBlock::~GrassBlock()
{
}

// ������
void GrassBlock::Initialize()
{
}

// �I����
void GrassBlock::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void GrassBlock::Begin()
{
}

// �X�V����
void GrassBlock::Update()
{

}

// Update�̌�ɌĂ΂��
void GrassBlock::End()
{
}

// �`�揈��
void GrassBlock::Render(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();

    // Transform�̍X�V
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��(�A�j���[�V�����Ȃ��z��)
    model->skinned_meshes.render(deviceContext, transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
}

//debug�p
void GrassBlock::DrawDebug()
{
    ImGui::Begin("GrassBlock");
    
    Block::DrawDebug();

    ImGui::End();
}
