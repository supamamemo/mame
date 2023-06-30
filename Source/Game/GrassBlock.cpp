#include "GrassBlock.h"

#include "../Mame/Graphics/Graphics.h"

#include <stdio.h>

int GrassBlock::nameNum = 0;

// �R���X�g���N�^
GrassBlock::GrassBlock()
{
    Graphics& graphics = Graphics::Instance();

    // model����
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_air_ground.fbx", true);

    // imgui���O���Ԃ�N���Ȃ��悤��...
    name = "GrassBlock" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

GrassBlock::GrassBlock(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    // model����
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/test.fbx", true);
    model = std::make_unique<Model>(graphics.GetDevice(), filename, true);

    // imgui���O���Ԃ�N���Ȃ��悤��...
    name = "GrassBlock" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// �f�X�g���N�^
GrassBlock::~GrassBlock()
{
}

// ������
void GrassBlock::Initialize(DirectX::XMFLOAT3 pos)
{
    model->GetTransform()->SetPosition(pos);
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
    model->skinned_meshes->render(deviceContext, transform, materialColor, nullptr);

    // BOUNDING_BOX
    {
#if 0
        DirectX::XMFLOAT4X4 t = SetDebugModelTransform(transform);
        debugModel->skinned_meshes.render(graphics.GetDeviceContext(), t, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.4f), nullptr);
#endif // _DEBUG
    }    
}

//debug�p
void GrassBlock::DrawDebug()
{
    ImGui::Begin(GetName());
    
    Block::DrawDebug();

    ImGui::End();
}