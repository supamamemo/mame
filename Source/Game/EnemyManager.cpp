#include "EnemyManager.h"
#include "../Game/Common.h"

// �X�V����
void EnemyManager::Update(float elapsedTime)
{
    // �X�V
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // �j������
    for (Enemy* enemy : removes)
    {
        // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

        // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
        // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        // �G�̔j������
        SafeDelete(enemy);
    }
    // �j�����X�g���N���A
    removes.clear();
}

// �`�揈��
void EnemyManager::Render(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(elapsedTime);
    }
}

// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

// �G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
    // �j�����X�g�ɒǉ�
    removes.insert(enemy);
}

// �G�l�~�[�S�폜
void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        SafeDelete(enemy);
    }
    enemies.clear();
    enemies.shrink_to_fit();    // vector�̗]���ȃ��������������֐�(C++11)
}

// �f�o�b�O
void EnemyManager::DrawDebug()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebug();
    }
}
