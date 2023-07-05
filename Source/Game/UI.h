#pragma once

#include "../sprite.h"

#include "spriteDissolve.h"

#include <string>
#include <memory>

class UI
{
public:
    UI(const wchar_t* filename);
    ~UI();

    void Render();
    void DrawDebug();

    void SetPosition(DirectX::XMFLOAT2 p) { spr.pos = p; }
    DirectX::XMFLOAT2 GetPosition() { return spr.pos; }
    void SetSize(DirectX::XMFLOAT2 s) { spr.size = s; }
    DirectX::XMFLOAT2 GetSize() { return spr.size; }

    void SetIsRender(bool r) { isRender = r; }
    bool GetIsRender() { return isRender; }

    // Imgui���O���h�~�̂��߂̂�� //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui���O���h�~�̂��߂̂�� //   

private:
    struct SpriteUi
    {
        DirectX::XMFLOAT2 pos;
        DirectX::XMFLOAT2 size;
    }spr;

private:
    std::unique_ptr<SpriteDissolve>uiSprite;
    //std::unique_ptr<Sprite>uiSprite;
    std::string name;

    bool isRender = false;
};

