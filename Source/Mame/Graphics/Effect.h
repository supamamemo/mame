#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>

// �G�t�F�N�g
class Effect
{
public:
    Effect(const char* filename);
    ~Effect() {};

    // �Đ�
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f }, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

    // ��~
    void Stop(Effekseer::Handle handle);

    // ���W�ݒ�
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

    // �X�P�[���ݒ�
    void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

    // debug
    void DrawDebug();

    // fadeout
    void FadeOutEffect(Effekseer::Handle handle, const float time);
    Effekseer::Handle FadeOutEffect(const DirectX::XMFLOAT3& position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 color, const float time);

public:
    void SetTimer(float t) { timer = t; }
    void AddTimer(float t) { timer += t; }
    float GetTimer() { return timer; }

    void SetPosition(DirectX::XMFLOAT3 p) { pos = p; }
    void SetScale(DirectX::XMFLOAT3 s) { scale = s; }
    void SetColor(DirectX::XMFLOAT4 c) { color = c; }

    DirectX::XMFLOAT3 GetPosition() { return pos; }
    DirectX::XMFLOAT3 GetScale() { return scale; }
    DirectX::XMFLOAT4 GetColor() { return color; }

    Effekseer::Handle handle;
private:
    Effekseer::EffectRef effekseerEffect;

    DirectX::XMFLOAT3 pos{ -2.5,2,10 };             // �ʒu
    DirectX::XMFLOAT3 scale{ 1.0f,1.0f,1.0f };      // �傫��
    DirectX::XMFLOAT4 color{ 0.0f,1.0f,0.0f,1.0f }; // �F

    int drawTime = 0;

    float timer = 4.5f; // ��������̂��
};

