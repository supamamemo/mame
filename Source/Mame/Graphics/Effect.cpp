#include "Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

int Effect::nameNum = 0;

// �R���X�g���N�^
Effect::Effect(const char* filename)
{
    // �G�t�F�N�g��ǂݍ��݂���O�Ƀ��b�N����
    // ���}���`�X���b�h��Effect���쐬�����DeviceContext�𓯎��A�N�Z�X����
    // �t���[�Y����\��������̂Ŕr�����䂷��
    std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());

    // Effekseer�̃��\�[�X��ǂݍ���
    // Effekseer��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ����ߕ����R�[�h���K�v
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    // Effekseer::Manager���擾
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    // Effekseer�G�t�F�N�g��ǂݍ���
    effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);

    // imgui���O���Ԃ�N���Ȃ��悤��...
    name = "Effect" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// �Đ�
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 color)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, position.x, position.y, position.z);
    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);

    // color�ݒ�
    {
        //Effekseer��color��0~255�炵��
        Effekseer::Color col{ static_cast<unsigned char>(color.x * 255),
            static_cast<unsigned char>(color.y * 255),
        static_cast<unsigned char>(color.z * 255),
        static_cast<unsigned char>(color.w * 255) };
        effekseerManager->SetAllColor(handle, col);
    }

    return handle;
}


// ��~
void Effect::Stop(Effekseer::Handle handle)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    effekseerManager->StopEffect(handle);
}

// ���W�w��
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

// �X�P�[���ݒ�
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}

void Effect::DrawDebug()
{
    ImGui::Begin(GetName());
    
    ImGui::DragInt("drawTime", &drawTime);

    ImGui::DragFloat3("pos", &pos.x);
    ImGui::DragFloat3("scale", &scale.x);

    DirectX::XMFLOAT3 r{};
    r.x = DirectX::XMConvertToDegrees(rotate.x);
    r.y = DirectX::XMConvertToDegrees(rotate.y);
    r.z = DirectX::XMConvertToDegrees(rotate.z);
    
    ImGui::DragFloat3("rotate", &r.x);

    rotate.x = DirectX::XMConvertToRadians(r.x);
    rotate.y = DirectX::XMConvertToRadians(r.y);
    rotate.z = DirectX::XMConvertToRadians(r.z);
    
    ImGui::ColorEdit4("color", &color.x);
    ImGui::DragFloat("timer", &timer);
    ImGui::End();
}


Effekseer::Handle Effect::FadeOutEffect(const DirectX::XMFLOAT3& position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 color, const float time)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();


    Effekseer::Vector3D p{ pos.x, pos.y, pos.z };

    Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, p, time);
    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);


    // color�ݒ�
    {
        //Effekseer��color��0~255�炵��
        Effekseer::Color col{ static_cast<unsigned char>(color.x * 255),
            static_cast<unsigned char>(color.y * 255),
        static_cast<unsigned char>(color.z * 255),
        static_cast<unsigned char>(color.w * 255) };
        effekseerManager->SetAllColor(handle, col);
    }

    return handle;
}

Effekseer::Handle Effect::FadeOutEffect(const DirectX::XMFLOAT3& position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotate, DirectX::XMFLOAT4 color, const float time)
{

    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();


    Effekseer::Vector3D p{ pos.x, pos.y, pos.z };

    Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, p, time);
    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
    

    effekseerManager->SetRotation(handle, rotate.x, rotate.y, rotate.z);

    // color�ݒ�
    {
        //Effekseer��color��0~255�炵��
        Effekseer::Color col{ static_cast<unsigned char>(color.x * 255),
            static_cast<unsigned char>(color.y * 255),
        static_cast<unsigned char>(color.z * 255),
        static_cast<unsigned char>(color.w * 255) };
        effekseerManager->SetAllColor(handle, col);
    }

    return handle;
}

void Effect::FadeOutEffect(Effekseer::Handle handle, const float time)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();


    Effekseer::Vector3D p{ pos.x, pos.y, pos.z };

    effekseerManager->Play(effekseerEffect, p, time);

    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
    // color�ݒ�
    {
        //Effekseer��color��0~255�炵��
        Effekseer::Color col{ static_cast<unsigned char>(1.0f * 255),
            static_cast<unsigned char>(0.0f * 255),
        static_cast<unsigned char>(0.0f * 255),
        static_cast<unsigned char>(1.0f * 255) };
        effekseerManager->SetAllColor(handle, col);
    }
}
