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

    void Initialize();                      // ú»
    void Finalize();                        // I¹»
    
    void Update(const float& elapsedTime);  // XV
    
    void Render(const float& elapsedTime);  // `æ
    void DrawDebug();                       // fobO`æ

public:
    void Register(UI* ui);  // UIo^ 
    void Remove(UI* ui);    // UIí  
    void Clear();              // UISí

public:
    const int GetUICount()const { return static_cast<int>(UIs.size()); }
    UI* GetUI(int index) { return UIs.at(index); }

private:
    std::vector<UI*> UIs;
    std::set<UI*>removes;
};

