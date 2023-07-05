#include "TerrainManager.h"
#include "../../Game/Common.h"


void TerrainManager::Initialize()
{
    for (Terrain*& terrain : terrains)
    {
        terrain->Initialize();
    }
}


void TerrainManager::Finalize()
{
    for (Terrain*& terrain : terrains)
    {
        terrain->Finalize();
    }
}

// �X�V����
void TerrainManager::Update(const float& elapsedTime)
{
    // �X�V
    for (Terrain*& terrain : terrains)
    {
        terrain->Update(elapsedTime);
    }

    // �j������
    for (Terrain* terrain : removes)
    {
        // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
        std::vector<Terrain*>::iterator it = std::find(terrains.begin(), terrains.end(), terrain);

        // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
        // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
        if (it != terrains.end())
        {
            terrains.erase(it);
        }

        // �G�̔j������
        SafeDelete(terrain);
    }
    // �j�����X�g���N���A
    removes.clear();
}

// �`�揈��
void TerrainManager::Render(const float& elapsedTime)
{
    for (Terrain*& terrain : terrains)
    {
        terrain->Render(elapsedTime);
    }
}

// �n�`�o�^
void TerrainManager::Register(Terrain* terrain)
{
    terrains.emplace_back(terrain);
}

// �n�`�폜
void TerrainManager::Remove(Terrain* terrain)
{
    // �j�����X�g�ɒǉ�
    removes.insert(terrain);
}

// �n�`�S�폜
void TerrainManager::Clear()
{
    for (Terrain*& terrain : terrains)
    {
        SafeDelete(terrain);
    }
    terrains.clear();
    terrains.shrink_to_fit();    // vector�̗]���ȃ��������������֐�(C++11)
}



// �f�o�b�O
void TerrainManager::DrawDebug()
{
    for (Terrain*& terrain : terrains)
    {
        terrain->DrawDebug();
    }
}
