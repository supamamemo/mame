#pragma once

#include <DirectXMath.h>
#include <memory>
#include <string>

#include "../sprite.h"

class SpriteAnimation
{
public:
    SpriteAnimation(const wchar_t* filename, const char* psFilename = nullptr);
    ~SpriteAnimation() {}

    void Initialize(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 texSize);

    /// <summary>
    /// �A�j���[�V�����X�V�֐�
    /// </summary>
    /// <param name="frameTime">�A�j���[�V�������鑬��</param>
    /// <param name="totalAnimationFrame">�A�j���[�V�����t���[����</param>
    /// <param name="animationVertical">�c�A�j���[�V�����̏ꍇtrue</param>
    void PlayAnimation(const int& frameTime, const int& totalAnimationFrame, bool animationVertical, const float elapsedTime);

    void Render();

    void DrawDebug();

    void SetPosition(DirectX::XMFLOAT2 p) { position = p; }
    void SetSize(DirectX::XMFLOAT2 s) { size = s; }
    void SetTexPos(DirectX::XMFLOAT2 texP) { texPos = texP; }
    void SetTexSize(DirectX::XMFLOAT2 texS) { texSize = texS; }
    void SetColor(DirectX::XMFLOAT4 c) { color = c; }
    void SetAngle(float a) { angle = a; }

    DirectX::XMFLOAT2 GetPosition() { return position; }
    DirectX::XMFLOAT2 GetSize() { return size; }
    DirectX::XMFLOAT2 GetTexPos() { return texPos; }
    DirectX::XMFLOAT2 GetTexSize() { return texSize; }
    DirectX::XMFLOAT4 GetColor() { return color; }
    float GetAngle() { return angle; }

private:
    DirectX::XMFLOAT2 position = {};
    DirectX::XMFLOAT2 size = {};
    DirectX::XMFLOAT2 texPos = {};
    DirectX::XMFLOAT2 texSize = {};
    DirectX::XMFLOAT4 color = { 1,1,1,1 };
    float angle = 0;
    float animationTime = 0;
    int animationFrame = 0;

    std::unique_ptr<Sprite> sprite = nullptr;

public:
    // Imgui���O���h�~�̂��߂̂�� //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui���O���h�~�̂��߂̂�� //
    
private:
    std::string name;   // Imgui�p
};

