#include "Terrain.h"
#include "../../Mame/Graphics/Graphics.h"
#include "../PlayerManager.h"
#include "TerrainManager.h"

int   Terrain::nameNum             = 0;
float Terrain::renderLengthXLimit_ = 40.0f;

Terrain::Terrain(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    {
        model_ = std::make_unique<Model>(graphics.GetDevice(), filename, true);

        create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/wireframe.cso", pixel_shaders.GetAddressOf());

        defaultMin_ = model_->skinned_meshes->boundingBox[0] * 0.01f;
        defaultMax_ = model_->skinned_meshes->boundingBox[1] * 0.01f;
        geometricAABB_ = std::make_unique<GeometricAABB>(graphics.GetDevice(), defaultMin_, defaultMax_);
        UpdateAABB();
    }

    // imgui���O���Ԃ�N���Ȃ��悤��...
    name = "Terrain" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}


void Terrain::Initialize()
{
    originPosition_ = GetTransform()->GetPosition();    // �����ʒu��ۑ�
}


void Terrain::Render(const float& elapsedTime)
{
    // DrawColl�����Ȃ����邽�߂�player����߂����̂����`�悷��
    {
        const float terrainPosX = GetTransform()->GetPosition().x;
        const float playerPosX  = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float lengthX     = fabsf(terrainPosX - playerPosX);

        if (lengthX > renderLengthXLimit_) return;
    }


    Graphics& graphics = Graphics::Instance();

    // world�s��X�V
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model_->GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��
    if (&model_->keyframe)
    {
        model_->skinned_meshes->render(graphics.GetDeviceContext(), transform, GetMaterialColor(), &model_->keyframe);
    }
    else
    {
        model_->skinned_meshes->render(graphics.GetDeviceContext(), transform, GetMaterialColor(), nullptr);
    }


#if _DEBUG

    // �f�o�b�O���f���ł̕`��
#if 0
    DirectX::XMFLOAT4X4 debugTransform = {};

    // ���[���h�s��̎擾�ƃX�P�[������
    DirectX::XMStoreFloat4x4(&debugTransform, debugModel->GetTransform()->CalcWorldMatrix(0.01f));

    // ���[���h�s��ݒ�
    debugTransform = SetDebugModelTransform(debugTransform);

    // �`��
    debugModel->skinned_meshes.render(graphics.GetDeviceContext(), debugTransform, { 1.0f, 0.0f, 0.0f, 0.2f }, nullptr);
#endif

    // ���X�^���C�U�X�e�[�g�쐬�E�ݒ�
#if 0
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state = nullptr;

    // ���X�^���C�U�X�e�[�g�쐬
    NO_CONST HRESULT hr = {};
    NO_CONST D3D11_RASTERIZER_DESC rasterizer_desc = {};
    //rasterizer_desc.FillMode                = D3D11_FILL_WIREFRAME; // �h��Ԃ�
    rasterizer_desc.FillMode = D3D11_FILL_SOLID; // �h��Ԃ�
    //rasterizer_desc.CullMode                = D3D11_CULL_BACK;	    // �w�ʃJ�����O�i���ʂ�`�悵�Ȃ��j
    rasterizer_desc.CullMode = D3D11_CULL_NONE;	    // �J�����O�Ȃ�

    // ?
    //rasterizer_desc.FrontCounterClockwise   = FALSE;			    // �O�p�`���𔽎��v���i�����j�ɂ��邩
    rasterizer_desc.FrontCounterClockwise = TRUE;			    // �O�p�`���𔽎��v���i�����j�ɂ��邩

    rasterizer_desc.DepthBias = 0;	                // �w�肳�ꂽ�s�N�Z���ɒǉ����ꂽ�[�x�l
    rasterizer_desc.DepthBiasClamp = 0;	                // �s�N�Z���̍ő�[�x�o�C�A�X
    rasterizer_desc.SlopeScaledDepthBias = 0;	                // �w�肳�ꂽ�s�N�Z���̌X���̃X�J���[
    rasterizer_desc.DepthClipEnable = TRUE;	                // �����ɂ��N���b�s���O���s�����̃t���O
    rasterizer_desc.ScissorEnable = FALSE;                // �V�U�[��`�J�����O���s�����̃t���O
    rasterizer_desc.MultisampleEnable = FALSE;                // �}���`�T���v�����O�A���`�G�C���A�X(���́FMSAA)�̃����_�[�^�[�Q�b�g���g�p���Ă��鎞�A�l�ӌ`���C���A���`�G�C���A�X���s�����A�A���t�@���C���A���`�G�C���A�X���s���������߂�t���O
    rasterizer_desc.AntialiasedLineEnable = FALSE;                // MSAA�̃����_�[�^�[�Q�b�g���g�p���Ă��鎞�A�����`���MultisampleEnable��false�̂Ƃ��A�A���`�G�C���A�X��L���ɂ��܂�
    hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizer_state.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    // ���X�^���C�U�X�e�[�g�ݒ�(���C���[�t���[��)
    graphics.GetDeviceContext()->RSSetState(rasterizer_state.Get());

#else
    // RS�ԍ�
    {
        // 0 �\���b�h�E���J�����O
        // 1 ���C���[�t���[���E���J�����O
        // 2 ���C���[�t���[���E�J�����O�Ȃ�
        // 3 �\���b�h�E�J�����O�Ȃ�
    }

    // ���X�^���C�U�ݒ�(�\���b�h�E�J�����O�Ȃ�)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 3, 0, 0);

#endif
    // ��]�Ȃ����[���h�s��̍쐬
    NO_CONST  DirectX::XMFLOAT4X4 noRotationTransform = {};
    {
        const DirectX::XMFLOAT3  scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
        const DirectX::XMFLOAT3& position = GetTransform()->GetPosition();
        const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
        const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMStoreFloat4x4(&noRotationTransform, S * T);
    }

    // �J���[�ݒ�
    //const DirectX::XMFLOAT4 materialColor = { 1, 0, 0, 0.4f };

    // AABB�`��
    //geometricAABB_->render(graphics.GetDeviceContext(), noRotationTransform, debugMaterialColor);

    // ���X�^���C�U�Đݒ�(�\���b�h�E���J�����O)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 0, 0, 0);

#endif // _DEBUG
}


void Terrain::DrawDebug()
{
    // ImGui�`��
    GetTransform()->DrawDebug();
}


void Terrain::Destroy()
{
    TerrainManager::Instance().Remove(this); // �������폜���X�g�ɒǉ�
}

void Terrain::UpdateAABB()
{
    const DirectX::XMFLOAT3& position = GetTransform()->GetPosition();

    aabb_.min = position + defaultMin_;
    aabb_.max = position + defaultMax_;
}
