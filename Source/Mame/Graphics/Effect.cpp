#include "Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

// �R���X�g���N�^
Effect::Effect(const char* filename)
{
    // Effekseer�̃��\�[�X��ǂݍ���
    // Effekseer��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ����ߕ����R�[�h���K�v
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    // Effekseer::Manager���擾
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    // Effekseer�G�t�F�N�g��ǂݍ���
    effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);
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
    ImGui::Begin("effect");
    
    ImGui::DragInt("drawTime", &drawTime);

    ImGui::DragFloat3("pos", &pos.x);
    ImGui::DragFloat3("scale", &scale.x);
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