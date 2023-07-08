#pragma once

#include "Enemy.h"
#include <vector>
#include <set>

// �G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    // �B��̃C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Initialize();                      // ������
    void Finalize();                        // �I����
    void Begin();                           // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);  // �X�V����
    void End();                             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime);  // �`�揈��
    void DrawDebug();                       // �f�o�b�O�`��

    void AllKill();

    void CollisionEnemyVsEnemy();           // �G�l�~�[���m�̏Փ˔��菈��
public:
    // �G�l�~�[�o�^
    void Register(Enemy* enemy);

    // �G�l�~�[�폜
    void Remove(Enemy* enemy);

    // �G�l�~�[�S�폜
    void Clear();

public:
    // �G�l�~�[���擾
    const int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    // �G�l�~�[�擾
    Enemy* GetEnemy(int index) { return enemies.at(index); }

private:
    std::vector<Enemy*> enemies = {};
    std::set<Enemy*>    removes = {};
};

