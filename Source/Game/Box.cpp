#include "Box.h"

#include "../Mame/Graphics/Graphics.h"

#include "PlayerManager.h"

int Box::nameNum = 0;
float Box::renderLengthXLimit_ = 40.0f;

// �R���X�g���N�^
Box::Box(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), filename, true);

    // imgui���O���Ԃ�N���Ȃ��悤��...
    name = "Box" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// ������
void Box::Initialize()
{
}

// �I����
void Box::Finalize()
{
}

// �X�V����
void Box::Update(const float& elapsedTime)
{
}

// �`�揈��
void Box::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // DrawColl�����Ȃ����邽�߂�player����߂����̂����`�悷��
    {
        const float& boxPosX    = GetTransform()->GetPosition().x;
        const float& playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float  lengthX    = fabsf(boxPosX - playerPosX);

        if (lengthX > renderLengthXLimit_) return;
    }

    // world�s��X�V
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model�`��
    if (&model->keyframe)
    {
        model->skinned_meshes->render(graphics.GetDeviceContext(), transform, GetMaterialColor(), &model->keyframe);
    }
    else
    {
        model->skinned_meshes->render(graphics.GetDeviceContext(), transform, GetMaterialColor(), nullptr);
    }
}

// debug�p
void Box::DrawDebug()
{
#ifdef USE_IMGUI
    ImGui::Begin(GetName());

    GetTransform()->DrawDebug();

    ImGui::ColorEdit4("color", &materialColor.x);

    ImGui::End();
#endif
}
