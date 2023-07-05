#pragma once

#include <memory>
#include "Terrain.h"

class TerrainManager
{
private:
    TerrainManager()  = default;
    ~TerrainManager() = default;
    
public:
    static TerrainManager& Instance()
    {
        static TerrainManager instance;
        return instance;
    }

    void Initialize();                      // ������
    void Finalize();                        // �I����
    void Update(const float& elapsedTime);  // �X�V����
    void Render(const float& elapsedTime);  // �`�揈��
    void DrawDebug();                       // �f�o�b�O�`��

public:
    void Register(Terrain* enemy);  // �n�`�o�^ 
    void Remove(Terrain* enemy);    // �n�`�폜  
    void Clear();                   // �n�`�S�폜

public: 
    const int GetTerrainCount() const { return static_cast<int>(terrains.size()); } // �n�`���擾  
    Terrain* GetTerrain(int index) { return terrains.at(index); }                   // �n�`�擾



private:
    std::vector<Terrain*> terrains;
    std::set<Terrain*> removes;
};

