#pragma once

#include "Enemy.h"
#include <vector>
#include <set>

// エネミーマネージャー
class EnemyManager
{
private:
    EnemyManager() {};
    ~EnemyManager() {};

public:
    // 唯一のインスタンス取得
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(float elapsedTime);

    // エネミー登録
    void Register(Enemy* enemy);

    // エネミー削除
    void Remove(Enemy* enemy);

    // エネミー全削除
    void Clear();

    void DrawDebug();

    // エネミー数取得
    int GetEnemyCount()const { return static_cast<int>(enemies.size()); }

    // エネミー取得
    Enemy* GetEnemy(int index) { return enemies.at(index); }

private:
    std::vector<Enemy*> enemies;
    std::set<Enemy*> removes;
};

