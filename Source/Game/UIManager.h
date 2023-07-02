#pragma once
#include "UI.h"

#include <memory>
#include <vector>
#include <set>

class UIManager
{
private:
    UIManager() = default;
    ~UIManager() = default;

public:
    static UIManager& Instance()
    {
        static UIManager instance;
        return instance;
    }

    void Initialize();                      // ������
    void Finalize();                        // �I����
    
    void Update(const float& elapsedTime);  // �X�V����
    
    void Render(const float& elapsedTime);  // �`�揈��
    void DrawDebug();                       // �f�o�b�O�`��

public:
    void Register(UI* ui);  // UI�o�^ 
    void Remove(UI* ui);    // UI�폜  
    void Clear();              // UI�S�폜

public:
    const int GetUICount()const { return static_cast<int>(UIs.size()); }
    UI* GetUI(int index) { return UIs.at(index); }

private:
    std::vector<UI*> UIs;
    std::set<UI*>removes;
};

