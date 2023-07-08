#include "UIManager.h"
#include "../Game/Common.h"

void UIManager::Initialize()
{
    SetUiCenter(false);
}

void UIManager::Finalize()
{
}



void UIManager::Update(const float& elapsedTime)
{
    for (UI*& ui : UIs)
    {
        // update
    }

    // �j������
    for (UI* ui : removes)
    {
        // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
        std::vector<UI*>::iterator it = std::find(UIs.begin(), UIs.end(), ui);

        // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
        // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
        if (it != UIs.end())
        {
            UIs.erase(it);
        }

        SafeDelete(ui);
    }
    removes.clear();
}



void UIManager::Render(const float& elapsedTime)
{
    for (UI*& ui : UIs)
    {
        if (ui->GetIsRender())ui->Render();
    }
}

void UIManager::DrawDebug()
{
    for (UI*& ui : UIs)
    {
        ui->DrawDebug();
    }
}

void UIManager::Register(UI* ui)
{
    UIs.emplace_back(ui);
}

void UIManager::Remove(UI* ui)
{
    removes.insert(ui);
}

void UIManager::Clear()
{
    for (UI*& ui : UIs)
    {
        SafeDelete(ui);
    }
    UIs.clear();
    UIs.shrink_to_fit();
}
