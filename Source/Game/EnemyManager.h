#pragma once

#include "Enemy.h"
#include <vector>
#include <set>

// �G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
    EnemyManager() {};
    ~EnemyManager() {};

public:
    // �B��̃C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(float elapsedTime);

    // �G�l�~�[�o�^
    void Register(Enemy* enemy);

    // �G�l�~�[�폜
    void Remove(Enemy* enemy);

    // �G�l�~�[�S�폜
    void Clear();

    void DrawDebug();

    // �G�l�~�[���擾
    int GetEnemyCount()const { return static_cast<int>(enemies.size()); }

    // �G�l�~�[�擾
    Enemy* GetEnemy(int index) { return enemies.at(index); }

private:
    std::vector<Enemy*> enemies;
    std::set<Enemy*> removes;
};

