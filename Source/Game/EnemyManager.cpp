#include "EnemyManager.h"
#include "../Game/Common.h"


void EnemyManager::Initialize()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Initialize();
    }
}


void EnemyManager::Finalize()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Finalize();
    }
}


void EnemyManager::Begin()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Begin();
    }
}


// 更新処理
void EnemyManager::Update(const float& elapsedTime)
{
    // 更新
    for (Enemy*& enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // 破棄処理
    for (Enemy* enemy : removes)
    {
        // vectorから要素を削除する場合はイテレーターで削除
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

        // std::vectorで管理されている要素を削除するにはerase()関数を使用する
        // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        // 敵の破棄処理
        SafeDelete(enemy);
    }
    // 破棄リストをクリア
    removes.clear();
}


void EnemyManager::End()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->End();
    }
}


// 描画処理
void EnemyManager::Render(const float& elapsedTime)
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Render(elapsedTime);
    }
}

// エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

// エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
    // 破棄リストに追加
    removes.insert(enemy);
}

// エネミー全削除
void EnemyManager::Clear()
{
    for (Enemy*& enemy : enemies)
    {
        SafeDelete(enemy);
    }
    enemies.clear();
    enemies.shrink_to_fit();    // vectorの余分なメモリを解放する関数(C++11)
}

// デバッグ
void EnemyManager::DrawDebug()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->DrawDebug();
    }
}
